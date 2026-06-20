#include <pch.h>
#include "Render/RenderGlobals.h"
#include "Render/GraphicsDevice.h"
#include "Render/Buffer.h"
#include "Render/Texture.h"
#include "Render/Framebuffer.h"

#include <SDL3/SDL_gpu.h>

namespace Cober {

// ======================================================================================
// Initialization
// ======================================================================================

void RenderGlobals::Init(void* window, void* context)
{
    // GraphicsDevice is initialized directly in Window.cpp.
    // This hook exists for compatibility — the real work happens in GraphicsDevice::Init().
    (void)window;
    (void)context;
}

void RenderGlobals::Clear()
{
    // No-op: clearing is handled via render pass load ops
}

// ======================================================================================
// Frame Lifecycle
// ======================================================================================

bool RenderGlobals::BeginFrame()
{
    return GraphicsDevice::Get().BeginFrame();
}

void RenderGlobals::BeginMainRenderPass()
{
    GraphicsDevice::Get().BeginSwapchainRenderPass();
}

void RenderGlobals::EndFrame()
{
    GraphicsDevice::Get().EndFrame();
}

// ======================================================================================
// Drawing
// ======================================================================================

// Provisional test function — draws a vertex buffer directly using the bound shader's
// default pipeline. In the future, all drawing should go through DrawIndexed with VertexArrays.
void RenderGlobals::DrawInternal(const Ref<VertexBuffer>& vertexBuffer)
{
    auto& gpu = GraphicsDevice::Get();
    if (!gpu.GetSwapchainTexture())
        return;

    SDL_GPUCommandBuffer* cmdBuf = gpu.GetCommandBuffer();

    // Upload dirty vertex data BEFORE starting the render pass.
    // SDL3 GPU requires all copy passes (uploads) to happen outside any render pass.
    vertexBuffer->EnsureUploaded(cmdBuf, true);

    // Now begin the render pass
    gpu.BeginSwapchainRenderPass();
    SDL_GPURenderPass* renderPass = gpu.GetRenderPass();
    if (!renderPass)
        return;

    // Get the bound shader to retrieve the pipeline
    const Shader* shader = Shader::GetBoundShader();
    if (!shader)
    {
        LOG_CORE_WARNING("DrawInternal: no shader bound — call Shader::Bind() first");
        return;
    }

    // Build a minimal pass signature matching the swapchain
    ShaderPassSignature sig{};
    sig.PrimitiveType = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    sig.NumColorTargets = 1;
    sig.ColorFormats[0] = (int)gpu.GetSwapchainFormat();
    sig.AlphaBlend = true;

    // Create a temporary VertexArray for pipeline creation (uses the buffer's layout)
    auto tempVA = VertexArray::Create();
    tempVA->AddVertexBuffer(vertexBuffer);

    // Get or create the pipeline for this vertex layout + target format
    SDL_GPUGraphicsPipeline* pipeline = const_cast<Shader*>(shader)->GetOrCreatePipeline(*tempVA, sig);
    if (!pipeline)
        return;

    // Bind the pipeline — this sets all fixed-function state
    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

    // Bind the vertex buffer
    auto* sdlVB = static_cast<SDL_GPUBuffer*>(vertexBuffer->GetGPUBuffer());
    SDL_GPUBufferBinding bufferBinding{};
    bufferBinding.buffer = sdlVB;
    bufferBinding.offset = 0;
    SDL_BindGPUVertexBuffers(renderPass, 0, &bufferBinding, 1);

    // Draw 3 vertices (a single triangle)
    SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
}

// Full-featured draw: uses VertexArray (vertex buffers + optional index buffer),
// the bound shader's pipeline cache, and bound uniforms/textures.
//
// This is the primary draw path for the engine. It:
//   1. Uploads any dirty vertex/index/texture data
//   2. Gets or creates the pipeline matching the vertex layout & render target
//   3. Binds pipeline, vertex buffers, index buffer
//   4. Pushes uniform data to vertex/fragment slots
//   5. Binds fragment textures and samplers
//   6. Issues the draw call
//
static void DrawWithVertexArray(const Ref<VertexArray>& vertexArray,
                                uint32_t count, uint32_t primitiveType, bool indexed)
{
    auto& gpu = GraphicsDevice::Get();
    SDL_GPUCommandBuffer* cmdBuf = gpu.GetCommandBuffer();
    if (!cmdBuf)
    {
        LOG_CORE_WARNING("DrawWithVertexArray: no command buffer");
        return;
    }

    const Shader* shader = Shader::GetBoundShader();
    if (!shader)
    {
        LOG_CORE_WARNING("DrawWithVertexArray: no shader bound");
        return;
    }

    // ---- UPLOAD PHASE: all GPU uploads MUST happen before any render pass begins ----
    // SDL3 GPU requires copy passes (used for uploading) to be separate from render passes.

    // 1. Upload dirty vertex buffers
    std::vector<SDL_GPUBufferBinding> vbBindings;
    vbBindings.reserve(vertexArray->GetVertexBuffers().size());

    for (const auto& vb : vertexArray->GetVertexBuffers())
    {
        vb->EnsureUploaded(cmdBuf, true);

        SDL_GPUBufferBinding binding{};
        binding.buffer = vb->GetGPUBuffer();
        binding.offset = 0;
        vbBindings.push_back(binding);
    }

    // 2. Upload dirty index buffer (if indexed)
    SDL_GPUBufferBinding ibBinding{};
    if (indexed)
    {
        auto ib = vertexArray->GetIndexBuffer();
        ib->EnsureUploaded(cmdBuf, false);
        ibBinding.buffer = ib->GetGPUBuffer();
        ibBinding.offset = 0;
    }

    // 3. Upload dirty textures
    for (uint32_t slot = 0; slot < shader->GetFragmentSamplerCount(); ++slot)
    {
        const Texture* texture = Texture::GetBound(slot);
        if (texture)
            const_cast<Texture*>(texture)->EnsureUploaded(cmdBuf, true);
    }

    // ---- RENDER PHASE: begin the render pass (if not already active) and draw ----

    // 4. Ensure a render pass is active
    SDL_GPURenderPass* renderPass = gpu.GetRenderPass();
    if (!renderPass)
    {
        gpu.BeginSwapchainRenderPass();
        renderPass = gpu.GetRenderPass();
    }
    if (!renderPass)
    {
        LOG_CORE_WARNING("DrawWithVertexArray: failed to begin render pass");
        return;
    }

    // 5. Build a pass signature matching the current render pass target
    ShaderPassSignature sig{};
    sig.PrimitiveType = primitiveType;
    sig.NumColorTargets = 1;
    sig.ColorFormats[0] = (int)gpu.GetSwapchainFormat();
    sig.AlphaBlend = true;
    sig.HasDepth = false;

    // 6. Get or create the pipeline
    SDL_GPUGraphicsPipeline* pipeline = const_cast<Shader*>(shader)->GetOrCreatePipeline(*vertexArray, sig);
    if (!pipeline)
        return;

    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

    // 7. Bind vertex buffers
    if (!vbBindings.empty())
        SDL_BindGPUVertexBuffers(renderPass, 0, vbBindings.data(), (uint32_t)vbBindings.size());

    // 8. Bind index buffer (if indexed)
    if (indexed)
        SDL_BindGPUIndexBuffer(renderPass, &ibBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    // 9. Push vertex uniform data
    for (uint32_t slot = 0; slot < shader->GetVertexUniformBufferCount(); ++slot)
    {
        const UniformBuffer* uniform = UniformBuffer::GetBound(slot);
        if (uniform && !uniform->GetBytes().empty())
        {
            SDL_PushGPUVertexUniformData(
                cmdBuf, slot,
                uniform->GetBytes().data(),
                (uint32_t)uniform->GetBytes().size()
            );
        }
    }

    // 10. Push fragment uniform data
    for (uint32_t slot = 0; slot < shader->GetFragmentUniformBufferCount(); ++slot)
    {
        const UniformBuffer* uniform = UniformBuffer::GetBound(slot);
        if (uniform && !uniform->GetBytes().empty())
        {
            SDL_PushGPUFragmentUniformData(
                cmdBuf, slot,
                uniform->GetBytes().data(),
                (uint32_t)uniform->GetBytes().size()
            );
        }
    }

    // 11. Bind fragment textures and samplers
    if (shader->GetFragmentSamplerCount() > 0)
    {
        std::vector<SDL_GPUTextureSamplerBinding> samplerBindings(shader->GetFragmentSamplerCount());
        for (uint32_t slot = 0; slot < shader->GetFragmentSamplerCount(); ++slot)
        {
            SDL_GPUTextureSamplerBinding texBinding{};
            texBinding.texture = Texture::GetRawBound(slot);
            texBinding.sampler = Texture::GetRawSampler(slot);
            samplerBindings[slot] = texBinding;
        }

        SDL_BindGPUFragmentSamplers(renderPass, 0, samplerBindings.data(), (uint32_t)samplerBindings.size());
    }

    // 12. Draw!
    if (indexed)
        SDL_DrawGPUIndexedPrimitives(renderPass, count, 1, 0, 0, 0);
    else
        SDL_DrawGPUPrimitives(renderPass, count, 1, 0, 0);
}

void RenderGlobals::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
{
    uint32_t drawCount = count;
    if (drawCount == 0)
    {
        auto ib = vertexArray->GetIndexBuffer();
        LOG_CORE_ASSERT(ib, "DrawIndexed requires an index buffer when count is 0");
        drawCount = ib->GetCount();
    }

    DrawWithVertexArray(vertexArray, drawCount, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, true);
}

void RenderGlobals::DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
{
    DrawWithVertexArray(vertexArray, vertexCount, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, false);
}

void RenderGlobals::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
{
    DrawWithVertexArray(vertexArray, vertexCount, SDL_GPU_PRIMITIVETYPE_LINELIST, false);
}

// ======================================================================================
// State Setters
// ======================================================================================

void RenderGlobals::SetViewport(uint32_t width, uint32_t height)
{
    GraphicsDevice::Get().SetViewport(0, 0, width, height);
}

void RenderGlobals::SetClearColor(glm::vec4 color)
{
    GraphicsDevice::Get().SetClearColor(color);
}

void RenderGlobals::SetClearColor(float r, float g, float b, float a)
{
    GraphicsDevice::Get().SetClearColor(r, g, b, a);
}

void RenderGlobals::SetLineWidth(float width)
{
    (void)width;
    // SDL3 GPU does not support line width natively — use line-list with wide lines via geometry
}

// ======================================================================================
// ImGui Integration
// ======================================================================================

void RenderGlobals::ImGuiInit()
{
    GraphicsDevice::Get().ImGuiInit();
}

void RenderGlobals::ImGuiShutdown()
{
    GraphicsDevice::Get().ImGuiShutdown();
}

void RenderGlobals::ImGuiNewFrame()
{
    GraphicsDevice::Get().ImGuiNewFrame();
}

void RenderGlobals::ImGuiPrepareDrawData(ImDrawData* drawData)
{
    GraphicsDevice::Get().ImGuiPrepareDrawData(drawData);
}

void RenderGlobals::ImGuiRenderDrawData(ImDrawData* drawData)
{
    GraphicsDevice::Get().ImGuiRenderDrawData(drawData);
}

} // namespace Cober

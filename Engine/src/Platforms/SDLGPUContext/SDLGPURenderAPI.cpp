
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"
#include "Platforms/SDLGPU/SDLGPUBuffer.h"
#include "Platforms/SDLGPU/SDLGPUVertexArray.h"
#include "Platforms/SDLGPU/SDLGPUTexture.h"
#include "Platforms/SDLGPU/SDLGPUShader.h"
#include "Platforms/SDLGPU/SDLGPUFramebuffer.h"
#include "Core/EngineApp.h"

#include <SDL3/SDL_gpu.h>

#include <imgui/imgui.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Cober 
{

    SDLGPURenderAPI* SDLGPURenderAPI::s_Instance = nullptr;

    // --------------------------------------------------------------------------------------
    
    void SDLGPURenderAPI::Init(void* window, void* context)
    {
        m_WindowHandle = static_cast<SDL_Window*>(window);
        LOG_CORE_ASSERT(m_WindowHandle, "Main window is null");

        m_GPUDevice = static_cast<SDL_GPUDevice*>(context);
        LOG_CORE_ASSERT(m_GPUDevice, "GPU device is null");

        s_Instance = this;
        LOG_CORE_TRACE("Render API init (SDL_GPU)");
    }

    // --------------------------------------------------------------------------------------

    bool SDLGPURenderAPI::BeginFrame()
    {
        m_Frame = {};
        m_Frame.CommandBuffer = SDL_AcquireGPUCommandBuffer(m_GPUDevice);
        LOG_CORE_ASSERT(m_Frame.CommandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

        const bool ok = SDL_WaitAndAcquireGPUSwapchainTexture(
            m_Frame.CommandBuffer,
            m_WindowHandle,
            &m_Frame.SwapchainTexture,
            nullptr,
            nullptr
        );
        LOG_CORE_ASSERT(ok, "SDL_WaitAndAcquireGPUSwapchainTexture failed: {0}", SDL_GetError());

        m_IsRenderingToSwapchain = false;
        return true;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::BeginMainRenderPass()
    {
        EnsureMainRenderPass();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::EndFrame()
    {
        EndActiveRenderPass();

        if (m_Frame.CommandBuffer)
        {
            SDL_SubmitGPUCommandBuffer(m_Frame.CommandBuffer);
        }

        m_Frame = {};
        m_IsRenderingToSwapchain = false;
    }

    // --------------------------------------------------------------------------------------
    
    void SDLGPURenderAPI::ImGuiInit()
    {
        LOG_CORE_ASSERT(m_WindowHandle, "ImGuiInit called before window creation");
        LOG_CORE_ASSERT(m_GPUDevice, "ImGuiInit called before GPU device creation");

        ImGui_ImplSDLGPU3_InitInfo initInfo = {};
        initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_GPUDevice, m_WindowHandle);
        initInfo.Device = m_GPUDevice;
        initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;

        const bool ok = ImGui_ImplSDLGPU3_Init(&initInfo);
        LOG_CORE_ASSERT(ok, "ImGui_ImplSDLGPU3_Init failed");

        m_ImGuiInitialized = true;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiShutdown()
    {
        if (!m_ImGuiInitialized)
        {
            return;
        }

        ImGui_ImplSDLGPU3_Shutdown();
        m_ImGuiInitialized = false;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiNewFrame()
    {
        LOG_CORE_ASSERT(m_ImGuiInitialized, "ImGuiNewFrame called before ImGuiInit");
        ImGui_ImplSDLGPU3_NewFrame();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiPrepareDrawData(ImDrawData* drawData)
    {
        if (EngineApp::Get().IsMinimized() || !drawData || !m_Frame.CommandBuffer)
        {
            return;
        }

        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, m_Frame.CommandBuffer);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiRenderDrawData(ImDrawData* drawData)
    {
        if (!drawData) {
            return;
        }

        EnsureMainRenderPass();
        if (!m_Frame.RenderPass) {
            return;
        }

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, m_Frame.CommandBuffer, m_Frame.RenderPass);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_Viewport = { x, y, width, height };
        ApplyViewport();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::SetClearColor(glm::vec4 color)
    {
        if (color.r > 1.0 && color.g > 1.0 && color.b > 1.0 && color.a > 1.0) {
            color /= glm::vec4(255.0f);
        }
        m_ClearColor = color;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::SetClearColor(float red, float green, float blue, float alpha)
    {
        glm::vec4 color { red, green, blue, alpha };
        if (red > 1.0 && green > 1.0 && blue > 1.0 && alpha > 1.0) {
            color /= glm::vec4(255.0f);
        }
        m_ClearColor = color;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        uint32_t drawCount = indexCount;
        if (drawCount == 0)
        {
            auto sdlva = std::dynamic_pointer_cast<SDLGPUVertexArray>(vertexArray);
            LOG_CORE_ASSERT(sdlva && sdlva->GetIndexBuffer(), "DrawIndexed requires an index buffer");
            drawCount = sdlva->GetIndexBuffer()->GetCount();
        }

        DrawInternal(vertexArray, drawCount, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, true);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
        DrawInternal(vertexArray, vertexCount, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, false);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
        DrawInternal(vertexArray, vertexCount, SDL_GPU_PRIMITIVETYPE_LINELIST, false);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::SetLineWidth(float width)
    {
        m_LineWidth = width;
        (void)m_LineWidth;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::BeginFramebufferRenderPass(SDLGPUFramebuffer* framebuffer)
    {
        LOG_CORE_ASSERT(framebuffer, "Framebuffer render pass requested with null framebuffer");
        EndActiveRenderPass();

        std::vector<SDL_GPUColorTargetInfo> colorInfos;
        colorInfos.reserve(framebuffer->GetColorAttachmentCount());

        for (uint32_t i = 0; i < framebuffer->GetColorAttachmentCount(); ++i)
        {
            SDL_GPUColorTargetInfo target = {};
            target.texture = framebuffer->GetColorAttachmentTexture(i);
            target.clear_color = (i == 0)
                ? SDL_FColor{ m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a }
                : SDL_FColor{ -1.0f, 0.0f, 0.0f, 0.0f };
            target.load_op = SDL_GPU_LOADOP_CLEAR;
            target.store_op = SDL_GPU_STOREOP_STORE;
            colorInfos.push_back(target);
        }

        SDL_GPUDepthStencilTargetInfo depthTarget = {};
        depthTarget.texture = framebuffer->GetDepthAttachmentTexture();
        depthTarget.clear_depth = 1.0f;
        depthTarget.load_op = SDL_GPU_LOADOP_CLEAR;
        depthTarget.store_op = SDL_GPU_STOREOP_STORE;
        depthTarget.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
        depthTarget.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
        depthTarget.clear_stencil = 0;
        depthTarget.cycle = false;

        m_Frame.RenderPass = SDL_BeginGPURenderPass(
            m_Frame.CommandBuffer,
            colorInfos.data(),
            (uint32_t)colorInfos.size(),
            depthTarget.texture ? &depthTarget : nullptr
        );
        LOG_CORE_ASSERT(m_Frame.RenderPass, "SDL_BeginGPURenderPass(framebuffer) failed: {0}", SDL_GetError());

        m_ActivePassSignature = {};
        m_ActivePassSignature.NumColorTargets = (uint32_t)framebuffer->GetColorAttachmentCount();
        for (uint32_t i = 0; i < m_ActivePassSignature.NumColorTargets; ++i)
        {
            m_ActivePassSignature.ColorFormats[i] = (i == 0) ? SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM : SDL_GPU_TEXTUREFORMAT_R32_INT;
        }
        m_ActivePassSignature.HasDepth = framebuffer->GetDepthAttachmentTexture() != nullptr;
        m_ActivePassSignature.DepthFormat = SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
        m_ActivePassSignature.Samples = 1;
        m_IsRenderingToSwapchain = false;

        ApplyViewport();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::EndActiveRenderPass()
    {
        if (m_Frame.RenderPass)
        {
            SDL_EndGPURenderPass(m_Frame.RenderPass);
            m_Frame.RenderPass = nullptr;
        }
    }

    // --------------------------------------------------------------------------------------

    SDLGPURenderAPI* SDLGPURenderAPI::Get()
    {
        return s_Instance;
    }
    
    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::EnsureMainRenderPass()
    {
        if (EngineApp::Get().IsMinimized() || m_Frame.RenderPass || !m_Frame.SwapchainTexture)
        {
            return;
        }

        SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
        colorTargetInfo.texture = m_Frame.SwapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{ m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        m_Frame.RenderPass = SDL_BeginGPURenderPass(m_Frame.CommandBuffer, &colorTargetInfo, 1, nullptr);
        LOG_CORE_ASSERT(m_Frame.RenderPass, "SDL_BeginGPURenderPass(swapchain) failed: {0}", SDL_GetError());

        UpdatePassSignatureForSwapchain();
        m_IsRenderingToSwapchain = true;
        ApplyViewport();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ApplyViewport()
    {
        if (!m_Frame.RenderPass || m_Viewport.z == 0 || m_Viewport.w == 0)
        {
            return;
        }

        SDL_GPUViewport viewport{};
        viewport.x = (float)m_Viewport.x;
        viewport.y = (float)m_Viewport.y;
        viewport.w = (float)m_Viewport.z;
        viewport.h = (float)m_Viewport.w;
        viewport.min_depth = 0.0f;
        viewport.max_depth = 1.0f;

        SDL_SetGPUViewport(m_Frame.RenderPass, &viewport);
    }

    // --------------------------------------------------------------------------------------
    
    void SDLGPURenderAPI::UpdatePassSignatureForSwapchain()
    {
        m_ActivePassSignature = {};
        m_ActivePassSignature.NumColorTargets = 1;
        m_ActivePassSignature.ColorFormats[0] = SDL_GetGPUSwapchainTextureFormat(m_GPUDevice, m_WindowHandle);
        m_ActivePassSignature.DepthFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
        m_ActivePassSignature.HasDepth = false;
        m_ActivePassSignature.Samples = 1;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::DrawInternal(const Ref<VertexArray>& vertexArray, uint32_t count, uint32_t primitiveType, bool indexed)
    {
        EnsureMainRenderPass();

        auto shader = SDLGPUShader::GetBoundShader();
        auto sdlva = std::dynamic_pointer_cast<SDLGPUVertexArray>(vertexArray);
        LOG_CORE_ASSERT(shader, "No shader is currently bound");
        LOG_CORE_ASSERT(sdlva, "Expected SDLGPUVertexArray");

        m_ActivePassSignature.PrimitiveType = primitiveType;
        // Blend the first target for regular sprite-like rendering, but keep the integer picking target untouched.
        m_ActivePassSignature.AlphaBlend = true;

        SDL_GPUGraphicsPipeline* pipeline = const_cast<SDLGPUShader*>(shader)->GetOrCreatePipeline(*sdlva, m_ActivePassSignature);
        SDL_BindGPUGraphicsPipeline(m_Frame.RenderPass, pipeline);

        std::vector<SDL_GPUBufferBinding> vbBindings;
        vbBindings.reserve(sdlva->GetVertexBuffers().size());

        for (const auto& vb : sdlva->GetVertexBuffers())
        {
            auto sdlvb = std::dynamic_pointer_cast<SDLGPUVertexBuffer>(vb);
            LOG_CORE_ASSERT(sdlvb, "Expected SDLGPUVertexBuffer");
            sdlvb->EnsureUploaded(m_Frame.CommandBuffer, true);

            SDL_GPUBufferBinding buffBinding{};
            buffBinding.buffer = sdlvb->GetGPUBuffer();
            buffBinding.offset = 0;
            vbBindings.push_back(buffBinding);
        }

        if (!vbBindings.empty())
        {
            SDL_BindGPUVertexBuffers(m_Frame.RenderPass, 0, vbBindings.data(), (uint32_t)vbBindings.size());
        }

        if (indexed)
        {
            auto sdlIndex = std::dynamic_pointer_cast<SDLGPUIndexBuffer>(sdlva->GetIndexBuffer());
            LOG_CORE_ASSERT(sdlIndex, "Expected SDLGPUIndexBuffer");
            sdlIndex->EnsureUploaded(m_Frame.CommandBuffer, false);

            SDL_GPUBufferBinding buffBinding {};
            buffBinding.buffer = sdlIndex->GetGPUBuffer();
            buffBinding.offset = 0;
            
            SDL_BindGPUIndexBuffer( m_Frame.RenderPass, &buffBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT );
        }

        for (uint32_t slot = 0; slot < shader->GetVertexUniformBufferCount(); ++slot)
        {
            const SDLGPUUniformBuffer* uniform = SDLGPUUniformBuffer::GetBound(slot);
            if (uniform && !uniform->GetBytes().empty())
            {
                SDL_PushGPUVertexUniformData(
                    m_Frame.CommandBuffer,
                    slot,
                    uniform->GetBytes().data(),
                    (uint32_t)uniform->GetBytes().size()
                );
            }
        }

        for (uint32_t slot = 0; slot < shader->GetFragmentUniformBufferCount(); ++slot)
        {
            const SDLGPUUniformBuffer* uniform = SDLGPUUniformBuffer::GetBound(slot);
            if (uniform && !uniform->GetBytes().empty())
            {
                SDL_PushGPUFragmentUniformData(
                    m_Frame.CommandBuffer,
                    slot,
                    uniform->GetBytes().data(),
                    (uint32_t)uniform->GetBytes().size()
                );
            }
        }

        if (shader->GetFragmentSamplerCount() > 0)
        {
            std::vector<SDL_GPUTextureSamplerBinding> samplerBindings(shader->GetFragmentSamplerCount());
            for (uint32_t slot = 0; slot < shader->GetFragmentSamplerCount(); ++slot)
            {
                const auto* texture = SDLGPUTexture::GetBound(slot);
                if (texture)
                {
                    const_cast<SDLGPUTexture*>(texture)->EnsureUploaded(m_Frame.CommandBuffer, true);
                }

                SDL_GPUTextureSamplerBinding textureBinding{};
                textureBinding.texture = SDLGPUTexture::GetRawBound(slot);
                textureBinding.sampler = SDLGPUTexture::GetRawSampler(slot);
                samplerBindings[slot] = textureBinding;
            }

            SDL_BindGPUFragmentSamplers(
                m_Frame.RenderPass,
                0,
                samplerBindings.data(),
                (uint32_t)samplerBindings.size()
            );
        }

        if (indexed) { 
            SDL_DrawGPUIndexedPrimitives(m_Frame.RenderPass, count, 1, 0, 0, 0); 
        }
        else {          
            SDL_DrawGPUPrimitives(m_Frame.RenderPass, count, 1, 0, 0); 
        }
    }

    // --------------------------------------------------------------------------------------
}

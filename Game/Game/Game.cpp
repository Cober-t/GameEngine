#include "Game.h"

// --------------------------------------------------------------------------------------
// Game Layer
//
// This is the main game application layer. It demonstrates the engine's rendering API
// by drawing a colored triangle using SDL3 GPU through the engine's abstractions.
//
// SDL3 GPU rendering flow (simplified):
//   1. Create shaders (compiled SPIR-V/DXIL/MSL bytecode)
//   2. Create vertex buffers with position/color data
//   3. On each frame:
//      a. AcquireGPUCommandBuffer + WaitAndAcquireSwapchainTexture
//      b. BeginRenderPass (clears the screen)
//      c. BindGraphicsPipeline + BindVertexBuffers + DrawPrimitives
//      d. EndRenderPass + SubmitGPUCommandBuffer
// --------------------------------------------------------------------------------------

Game::Game() : Layer("Game application")
{
}

// --------------------------------------------------------------------------------------

void Game::OnAttach()
{
    // Step 1: Create a shader.
    // This loads pre-compiled SPIR-V shaders from assets/shaders/compiled/.
    // The shader system also caches SDL_GPUGraphicsPipeline objects that bundle
    // the shaders with vertex layout descriptions and render target formats.
    m_Shader = Shader::Create("PositionColor");
    m_Shader->Bind();

    // Step 2: Create a vertex buffer and fill it with triangle data.
    // The VertexBuffer uses SDL3 GPU's SDL_CreateGPUBuffer with VERTEX usage.
    // Data is stored in a shadow copy and uploaded to the GPU automatically
    // when EnsureUploaded() is called (done inside DrawInternal).
    const uint32_t vertexCount = 3;
    const uint32_t bufferSize = vertexCount * sizeof(PositionColorVertex);

    m_VertexBuffer = VertexBuffer::Create(bufferSize);

    // Define a red-green-blue triangle
    PositionColorVertex vertices[3];
    vertices[0].Position = { -0.5f, -0.5f, 0.0f };
    vertices[1].Position = {  0.5f, -0.5f, 0.0f };
    vertices[2].Position = {  0.0f,  0.5f, 0.0f };
    vertices[0].Color = { 1.0f, 0.0f, 0.0f, 1.0f };  // Red
    vertices[1].Color = { 0.0f, 1.0f, 0.0f, 1.0f };  // Green
    vertices[2].Color = { 0.0f, 0.0f, 1.0f, 1.0f };  // Blue

    m_VertexBuffer->SetData(vertices, bufferSize);

    // Set the vertex buffer layout so the shader knows how to interpret the data.
    // This layout is used when creating the SDL_GPUGraphicsPipeline.
    m_VertexBuffer->SetLayout({
        { ShaderDataType::Float3, "Position" },
        { ShaderDataType::Float4, "Color"    },
    });
}

// --------------------------------------------------------------------------------------

void Game::OnDetach()
{
    // Resources (VertexBuffer, Shader) are ref-counted and cleaned up automatically.
    LOG_INFO("Detached Game application Layer!");
}

// --------------------------------------------------------------------------------------

void Game::OnUpdate(const Timestep& ts)
{
    (void)ts;

    // Draw the triangle each frame.
    // The frame lifecycle (BeginFrame / EndFrame) is managed by EngineApp::RunRender,
    // which acquires the command buffer + swapchain texture before calling OnUpdate
    // and submits the command buffer after.
    //
    // DrawInternal starts the swapchain render pass, binds the pipeline and buffers,
    // and issues the draw call. The render pass is ended by RunRender's EndFrame().

    RenderGlobals::DrawInternal(m_VertexBuffer);
}

// --------------------------------------------------------------------------------------

void Game::OnEvent(Event& event)
{
    if (event.GetEventType() == EventType::WindowResize)
    {
        auto& resizeEvent = static_cast<WindowResizeEvent&>(event);
        LOG_CORE_INFO("Window resized: {0} x {1}", resizeEvent.GetWidth(), resizeEvent.GetHeight());
    }
}

// --------------------------------------------------------------------------------------

#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "Core/Core.h"
#include <glm/glm.hpp>

// --------------------------------------------------------------------------------------
// SDL3 GPU forward declarations
// SDL3 GPU is a cross-platform abstraction over Vulkan, Direct3D 12, Metal,
// and OpenGL ES 3.0. The backend is selected automatically based on the shader
// formats you provide at device creation time.
// --------------------------------------------------------------------------------------
struct SDL_Window;
struct SDL_GPUDevice;
struct SDL_GPUCommandBuffer;
struct SDL_GPUTexture;
struct SDL_GPURenderPass;
struct ImDrawData;

namespace Cober {

// GraphicsDevice manages the SDL_GPUDevice lifecycle and per-frame operations.
//
// SDL3 GPU rendering flow:
//   1. AcquireCommandBuffer()  - get a buffer to record commands into
//   2. WaitAndAcquireSwapchainTexture() - get the texture for the current window
//   3. BeginRenderPass()       - start a scope of draw calls targeting a texture
//   4. BindPipeline / BindBuffers / DrawPrimitives - record draw calls
//   5. EndRenderPass()         - finish the render pass
//   6. SubmitCommandBuffer()   - send recorded commands to the GPU
//
class CB_API GraphicsDevice {
public:
    static void Init(SDL_Window* window);
    static void Shutdown();
    static GraphicsDevice& Get();
    static bool IsAlive() { return s_Instance != nullptr; }

    // === Frame Lifecycle ===
    // Begins a new frame: acquires a command buffer + swapchain texture.
    // Must be called once per frame before any rendering.
    bool BeginFrame();

    // Ends the current frame: ends any active render pass, submits the command buffer.
    void EndFrame();

    // === Render Pass Management ===
    // Begins a render pass on the swapchain texture (the window's surface).
    // Uses the current clear color. If a pass is already active, this is a no-op.
    // All draw calls must happen inside a render pass.
    void BeginSwapchainRenderPass();

    // Begins a render pass targeting an offscreen framebuffer's attachments.
    void BeginFramebufferRenderPass(class Framebuffer* framebuffer);

    // Ends the currently active render pass (swapchain or framebuffer).
    void EndActiveRenderPass();

    // === State Setters ===
    void SetClearColor(glm::vec4 color);
    void SetClearColor(float r, float g, float b, float a = 255.0f);
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    // === ImGui Integration ===
    void ImGuiInit();
    void ImGuiShutdown();
    void ImGuiNewFrame();
    void ImGuiPrepareDrawData(ImDrawData* drawData);
    void ImGuiRenderDrawData(ImDrawData* drawData);

    // === Accessors ===
    SDL_GPUDevice* GetDevice() const { return m_Device; }
    SDL_GPUCommandBuffer* GetCommandBuffer() const { return m_Frame.CommandBuffer; }
    SDL_GPUTexture* GetSwapchainTexture() const { return m_Frame.SwapchainTexture; }
    SDL_GPURenderPass* GetRenderPass() const { return m_Frame.RenderPass; }
    SDL_GPUTextureFormat GetSwapchainFormat() const { return m_SwapchainFormat; }

    struct RenderTargetInfo {
        uint32_t NumColorTargets = 0;
        int ColorFormats[4] = {};
        bool HasDepth = false;
        int DepthFormat = 0;
    };
    const RenderTargetInfo& GetRenderTargetInfo() const { return m_ActiveRenderTarget; }

private:
    GraphicsDevice() = default;

    void ApplyViewport();
    void UpdatePassSignatureForSwapchain();

    struct FrameState {
        SDL_GPUCommandBuffer* CommandBuffer = nullptr;
        SDL_GPUTexture* SwapchainTexture = nullptr;
        SDL_GPURenderPass* RenderPass = nullptr;
    };

    SDL_Window* m_Window = nullptr;
    SDL_GPUDevice* m_Device = nullptr;
    FrameState m_Frame;
    SDL_GPUTextureFormat m_SwapchainFormat = SDL_GPU_TEXTUREFORMAT_INVALID;

    glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    glm::uvec4 m_Viewport = { 0, 0, 0, 0 };
    bool m_ImGuiInitialized = false;
    bool m_RenderingToSwapchain = false;
    RenderTargetInfo m_ActiveRenderTarget;

    // Transfer buffers whose release is deferred until after command buffer submission
    std::vector<SDL_GPUTransferBuffer*> m_PendingTransferBuffers;

    static GraphicsDevice* s_Instance;
};

}

#endif

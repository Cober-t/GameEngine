
#ifndef SDLGPU_RENDER_API_H
#define SDLGPU_RENDER_API_H

#include "Render/RenderAPI.h"
#include "Platforms/SDLGPU/SDLGPUShader.h"

struct SDL_Window;
struct SDL_GPUDevice;
struct SDL_GPUCommandBuffer;
struct SDL_GPUTexture;
struct SDL_GPURenderPass;
struct ImDrawData;

namespace Cober {

    class SDLGPUFramebuffer;

    class SDLGPURenderAPI : public RenderAPI
    {
    public:
        void Init(void* window, void* context) override;

        void Clear() override;

        bool BeginFrame() override;
        void BeginMainRenderPass() override;
        void EndFrame() override;

        // Dear ImGui renderer-backend hooks
        void ImGuiInit() override;
        void ImGuiShutdown() override;
        void ImGuiNewFrame() override;
        void ImGuiPrepareDrawData(ImDrawData* drawData) override;
        void ImGuiRenderDrawData(ImDrawData* drawData) override;

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(glm::vec4 color) override;
        void SetClearColor(float red, float green, float blue, float black) override;

        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
        void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
        void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

        void SetLineWidth(float width) override;

        void ClearErrors() override {}
        void CheckErrors(const char* function) override {}

        void BeginFramebufferRenderPass(SDLGPUFramebuffer* framebuffer);
        void EndActiveRenderPass();

        SDL_GPUDevice* GetDevice() const { return m_GPUDevice; }
        SDL_GPUCommandBuffer* GetCommandBuffer() const { return m_Frame.CommandBuffer; }
        SDL_GPUTexture* GetSwapchainTexture() const { return m_Frame.SwapchainTexture; }

        static SDLGPURenderAPI* Get();

    private:
        struct FrameState
        {
            SDL_GPUCommandBuffer* CommandBuffer = nullptr;
            SDL_GPUTexture* SwapchainTexture = nullptr;
            SDL_GPURenderPass* RenderPass = nullptr;
        };

        void EnsureMainRenderPass();
        void ApplyViewport();
        void UpdatePassSignatureForSwapchain();
        void DrawInternal(const Ref<VertexArray>& vertexArray, uint32_t count, uint32_t primitiveType, bool indexed);

    private:
        FrameState m_Frame;
        SDL_Window* m_WindowHandle = nullptr;
        SDL_GPUDevice* m_GPUDevice = nullptr;

        bool m_ImGuiInitialized = false;
        glm::vec4 m_ClearColor = { 0.10f, 0.10f, 0.10f, 1.0f };
        glm::uvec4 m_Viewport = { 0, 0, 0, 0 };
        float m_LineWidth = 1.0f;

        SDLGPUShaderPassSignature m_ActivePassSignature;
        bool m_IsRenderingToSwapchain = false;

        static SDLGPURenderAPI* s_Instance;
    };
}

#endif

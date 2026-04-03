#ifndef SDLGPU_CONTEXT_H
#define SDLGPU_CONTEXT_H

#include "Render/GraphicsContext.h"

struct SDL_Window;
struct SDL_GPUDevice;

namespace Cober {

    class SDLGPUContext : public GraphicsContext
    {
    public:
        SDLGPUContext(SDL_Window* windowHandle, RenderAPI::API requestedAPI);
        ~SDLGPUContext() override = default;

        void Init() override;
        void SwapBuffers() override {};
        void Destroy() override;

        SDL_GPUDevice* GetGPUDevice() override { return m_GPUDevice; }
        RenderAPI::API GetRendererAPI() const override { return m_RequestedAPI; }
        const char* GetBackendName() const override;

        void ImGuiInit(SDL_Window* window) override;
        void ImGuiShutdown() override;
        void ImGuiNewFrame() override;
        void ImGuiRenderDrawData(ImDrawData* drawData) override;

    private:
        static const char* ToPreferredDriver(RenderAPI::API api);
        static Uint32 GetShaderFormatFlags();

    private:
        SDL_Window* m_WindowHandle = nullptr;
        SDL_GPUDevice* m_GPUDevice = nullptr;
        RenderAPI::API m_RequestedAPI = RenderAPI::API::SDLGPU;
        bool m_ImGuiInitialized = false;
    };

}

#endif
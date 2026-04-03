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

        virtual void* GetWindowHandle() override { return (void*)m_WindowHandle; };
        virtual void* GetDevice() override { return (void*)m_GPUDevice; };

        void Init() override;
        void SwapBuffers() override {};
        void Destroy() override;

    private:
        SDL_Window* m_WindowHandle = nullptr;
        SDL_GPUDevice* m_GPUDevice = nullptr;
    };
}

#endif
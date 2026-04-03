#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "Core/Core.h"
#include "Render/RenderAPI.h"

struct SDL_Window;
struct SDL_GPUDevice;
struct ImDrawData;

namespace Cober {

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Destroy() = 0;

        virtual SDL_GPUDevice* GetGPUDevice() = 0;
        virtual RenderAPI::API GetRendererAPI() const = 0;
        virtual const char* GetBackendName() const = 0;

        // Dear ImGui renderer-backend hooks
        virtual void ImGuiInit(SDL_Window* window) = 0;
        virtual void ImGuiShutdown() = 0;
        virtual void ImGuiNewFrame() = 0;
        virtual void ImGuiRenderDrawData(ImDrawData* drawData) = 0;

        static Unique<GraphicsContext> Create(void* window);
    };

}

#endif
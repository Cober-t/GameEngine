#include <pch.h>
#include "Render/GraphicsContext.h"
#include "Render/RenderAPI.h"

#include "Platforms/SDLGPU/SDLGPUContext.h"

namespace Cober {

    Unique<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:
                LOG_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;

            case RenderAPI::API::SDLGPU:
            case RenderAPI::API::Vulkan:
            case RenderAPI::API::Metal:
            case RenderAPI::API::DirectX12:
                return CreateUnique<SDLGPUContext>(static_cast<SDL_Window*>(window), RenderAPI::GetAPI());

            default:
                LOG_CORE_ASSERT(false, "Unknown RenderAPI!");
                return nullptr;
        }
    }
}
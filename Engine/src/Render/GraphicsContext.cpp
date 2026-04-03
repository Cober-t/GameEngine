#include <pch.h>
#include "Render/GraphicsContext.h"
#include "Render/RenderAPI.h"

#include "Render/GraphicsContext.h"
#include "Platforms/SDLGPU/SDLGPUContext.h"
// #include "Platforms/Vulkan/VulkanContext.h"

namespace Cober {

    Unique<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:
                LOG_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;

            case RenderAPI::API::SDLGPU:
                return CreateUnique<SDLGPUContext>(static_cast<SDL_Window*>(window), RenderAPI::API::SDLGPU);
            // Native implementations
            // case RenderAPI::API::Vulkan:
            //     return CreateUnique<VulkanContext>(static_cast<SDL_Window*>(window), RenderAPI::API::Vulkan);
            // case RenderAPI::API::Metal:
            // case RenderAPI::API::DirectX12:

            default:
                LOG_CORE_ASSERT(false, "Unknown RenderAPI!");
                return nullptr;
        }
    }
}
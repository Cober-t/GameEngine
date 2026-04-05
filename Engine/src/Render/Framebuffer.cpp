
#include <pch.h>
#include "Render/RenderAPI.h"
#include "Render/Framebuffer.h"

#include "Platforms/SDLGPU/SDLGPUFramebuffer.h"

namespace Cober {

    Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height)
    {
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:      LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUFramebuffer>(width, height);
            default: break;
        }
        LOG_CORE_ASSERT(false, "Unknown Framebuffer RenderAPI!");
        return nullptr;
    }
}

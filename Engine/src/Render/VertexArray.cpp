
#include <pch.h>
#include "Render/RenderAPI.h"
#include "Render/VertexArray.h"

#include "Platforms/SDLGPU/SDLGPUVertexArray.h"

namespace Cober {

    Ref<VertexArray> VertexArray::Create()
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:      LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUVertexArray>();
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }
}

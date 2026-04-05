
#include <pch.h>
#include "Render/Buffer.h"
#include "Render/RenderAPI.h"

#include "Platforms/SDLGPU/SDLGPUBuffer.h"

namespace Cober {

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:      LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUVertexBuffer>(size);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:      LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUVertexBuffer>(vertices, size);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:      LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUIndexBuffer>(indices, size);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (RenderAPI::GetAPI())
        {
            case RenderAPI::API::None:      LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::SDLGPU:    return CreateRef<SDLGPUUniformBuffer>(size, binding);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }
}

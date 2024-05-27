#include <pch.h>
#include "Buffer.h"
#include "Render/RenderAPI.h"

#include "Platforms/OpenGL/OpenGLBuffer.h"


namespace Cober {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLVertexBuffer>(size);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLVertexBuffer>(vertices, size);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLIndexBuffer>(indices, size);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}


	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:    LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
            default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}
}
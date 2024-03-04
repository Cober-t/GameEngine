#include "Render/RenderAPI.h"
#include "VertexArray.h"

#include "Platforms/OpenGL/OpenGLVertexArray.h"


namespace Cober {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:    LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}
}
#include <pch.h>
#include "Render/RenderAPI.h"
#include "Texture.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Cober {

	Ref<Texture> Texture::Create(uint32_t width, uint32_t height)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLTexture>(width, height);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}
		
		return nullptr;
	}
}
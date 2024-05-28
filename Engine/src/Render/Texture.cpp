#include <pch.h>
#include "Render/RenderAPI.h"
#include "Texture.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Cober {

	Ref<Texture> Texture::Create(const TextureSpecification& specification)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLTexture>(specification);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}
		
		return nullptr;
	}


	Ref<Texture> Texture::Create(const std::string& path)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    return CreateRef<OpenGLTexture>(path);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}
		
		return nullptr;
	}
}
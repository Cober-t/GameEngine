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


	SubTexture::SubTexture()
	{
		m_TexCoords[0] = { 0.0f, 0.0f };
		m_TexCoords[1] = { 1.0f, 0.0f };
		m_TexCoords[2] = { 1.0f, 1.0f };
		m_TexCoords[3] = { 0.0f, 1.0f };
	}

	SubTexture::SubTexture(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}


    Ref<SubTexture> SubTexture::CreateFromCoords(const Ref<Texture> texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize)
    {
		glm::vec2 min = { 
			(coords.x * cellSize.x) / texture->GetWidth(), 
			(coords.y * cellSize.y) / texture->GetHeight() 
		};
		glm::vec2 max = { 
			((coords.x +  spriteSize.x) * cellSize.x) / texture->GetWidth(), 
			((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() 
		};

		return CreateRef<SubTexture>(texture, min, max);
    }
}
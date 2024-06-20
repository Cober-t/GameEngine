#include <pch.h>
#include "Render/RenderAPI.h"
#include "Texture.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Cober {

	std::unordered_map<std::filesystem::path, Ref<Texture>> Texture::m_TexturesDataHolder;

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


	Ref<Texture> Texture::Create(const std::filesystem::path& path)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:      LOG_CORE_WARNING("RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:    
				if (m_TexturesDataHolder.find(path) == m_TexturesDataHolder.end())
				{
					m_TexturesDataHolder[path] = CreateRef<OpenGLTexture>(path);
				}
				return m_TexturesDataHolder[path];
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


    Ref<SubTexture> SubTexture::UpdateCoords(const Ref<Texture> texture, glm::mat4& vertices, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize)
    {
		// FIXME: Export to Settings and Editor
		float scaleFactor = 100;

		float textWidth = cellSize.x;
		float textHeight = cellSize.y;

		if (spriteSize.x != 1.0f || spriteSize.y != 1.0f)
		{
			textWidth = cellSize.x * spriteSize.x;
			textHeight = cellSize.y * spriteSize.y;
		}

		// Handle the size of the entity rendered and his rigidbody collider's size
		vertices = 
		{
			{ -textWidth/2/scaleFactor, -textHeight/2/scaleFactor, 0.0f, 1.0f },
			{  textWidth/2/scaleFactor, -textHeight/2/scaleFactor, 0.0f, 1.0f },
			{  textWidth/2/scaleFactor,  textHeight/2/scaleFactor, 0.0f, 1.0f },
			{ -textWidth/2/scaleFactor,  textHeight/2/scaleFactor, 0.0f, 1.0f }
		};
		texture->SetTextureVertices(vertices);

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
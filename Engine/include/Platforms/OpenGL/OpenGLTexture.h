#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Render/Texture.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"

#include <stb_image.h>
typedef unsigned int GLenum;

namespace Cober {

	struct TextureData
	{
		stbi_uc* data;
		int width;
		int height;
		int channels;
		uint64_t references = 0;

		TextureData() = default;
		TextureData(stbi_uc* d, int w, int h, int c) :
		 data(d), width(w), height(h), channels(c) {}
		
		~TextureData()
		{
			if (--references <= 0)
				stbi_image_free(data);
		}
	};

	class OpenGLTexture : public Texture
	{

	public:
		OpenGLTexture(const std::filesystem::path& path);
		OpenGLTexture(const TextureSpecification& specification);
		virtual ~OpenGLTexture();

		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual std::string GetName()   const override;
		virtual std::string GetFormat() const override;
		virtual const std::filesystem::path& GetPath() const override { return m_Path; }
		virtual const glm::mat4 GetTextureVertices() const override { return m_TextureVertices;};
		virtual void SetTextureVertices(const glm::mat4 vertices) { m_TextureVertices = vertices; };
		
		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		TextureSpecification m_Specification;

		std::filesystem::path m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
		
		glm::mat4 m_TextureVertices;
	};
}

#endif
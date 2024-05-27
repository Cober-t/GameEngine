#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Render/Texture.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"

#include <stb_image.h>
typedef unsigned int GLenum;

namespace Cober {

	class OpenGLTexture : public Texture
	{

	public:
		OpenGLTexture(const std::string& path);
		OpenGLTexture(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		
		virtual std::string GetName()   const override;
		virtual std::string GetFormat() const override;
		virtual const std::string& GetPath() const override { return m_Path; }
		
		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		uint32_t m_Width, m_Height;
		ImageFormat m_Format = ImageFormat::RGBA8;
		bool m_GenerateMips = true;
		std::string m_Path;

		bool m_IsLoaded = false;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}

#endif
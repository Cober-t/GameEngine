#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Render/Texture.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"

#include <stb_image/stb_image.h>

namespace Cober {

	class OpenGLTexture : public Texture
	{

	public:
		OpenGLTexture(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth()  const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void SetWidth(uint32_t width)   override { _width = width; }
		virtual void SetHeight(uint32_t height) override { _height = height; }

		virtual uint32_t GetID() const override { return m_RendererID; }
		virtual std::string GetName()   const override;
		virtual std::string GetFormat() const override;
		virtual std::string GetPath() const override { return _path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture&)other).m_RendererID;
		}

	private:
		std::string _path;
		uint32_t _width, _height;
		uint32_t m_RendererID;
		GLenum _internalFormat, _dataFormat;

		glm::vec2 subTexIndex[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	};
}

#endif
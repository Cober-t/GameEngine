#include <pch.h>
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"


namespace Cober {

	namespace Utils {

		static GLenum ImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8:  return GL_RGB;
				case ImageFormat::RGBA8: return GL_RGBA;
			}

			LOG_CORE_ASSERT(false, "Unknown texture format");
			return 0;
		}
		
		static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			LOG_CORE_ASSERT(false, "Unknown internal texture format");
			return 0;
		}

	}


	OpenGLTexture::OpenGLTexture(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		m_InternalFormat = Utils::ImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::ImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Specification.Width, m_Specification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	OpenGLTexture::OpenGLTexture(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			if (channels == 3 && m_Width%channels != 0)
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}


	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}


	std::string OpenGLTexture::GetName() const  
	{
		auto lastSlash = m_Path.find_last_of("/\\");
		
        LOG_CORE_ASSERT(lastSlash != std::string::npos, "Texture Path is invalid or does not exist!");
		
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = m_Path.rfind('.');
		
        LOG_CORE_ASSERT(lastDot != std::string::npos, "Texture Name is invalid or does not exist!");
		
		auto count = lastDot == std::string::npos ? m_Path.size() - lastSlash : lastDot - lastSlash;
		std::string name = m_Path.substr(lastSlash, count);
		return name;
	}


	std::string OpenGLTexture::GetFormat() const 
	{
		auto lastDot = m_Path.rfind('.');
		// auto count = lastDot != std::string::npos ? m_Path.size() - lastDot : m_Path.size();
		std::string format = lastDot != std::string::npos ? m_Path.substr(lastDot) : "null";
		return format;
	}


	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        LOG_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}


	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
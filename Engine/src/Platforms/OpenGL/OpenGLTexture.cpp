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

		static GLenum ImageFilterToGL(ImageFilter format)
		{
			switch (format)
			{
			case ImageFilter::NEAREST:  return GL_NEAREST;
			case ImageFilter::LINEAR: return GL_LINEAR;
			}

			LOG_CORE_ASSERT(false, "Unknown internal texture format");
			return 0;
		}

		static GLenum ImageRepeatPatternToGL(RepeatPattern format)
		{
			switch (format)
			{
			case RepeatPattern::REPEAT:  return GL_REPEAT;
			case RepeatPattern::CLAM_TO_EDGE: return GL_CLAMP_TO_EDGE;
			case RepeatPattern::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
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
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::ImageFilterToGL(m_Specification.Filter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::ImageFilterToGL(m_Specification.Filter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::ImageRepeatPatternToGL(m_Specification.Pattern));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::ImageRepeatPatternToGL(m_Specification.Pattern));

		uint32_t whiteTextureData = 0xffffffff;
		SetData(&whiteTextureData, sizeof(uint32_t));
	}


	OpenGLTexture::OpenGLTexture(const std::filesystem::path& path)
		: m_Path(path)
	{
		stbi_uc* data = nullptr;
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

		if (data)
		{
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
		}
	}


	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}


	std::string OpenGLTexture::GetName() const  
	{
		if (std::filesystem::exists(m_Path))
			return m_Path.filename().string();
		else;
			return "";
	}


	std::string OpenGLTexture::GetFormat() const 
	{
		if (std::filesystem::exists(m_Path))
			return m_Path.extension().string();
		else;
			return "null";
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
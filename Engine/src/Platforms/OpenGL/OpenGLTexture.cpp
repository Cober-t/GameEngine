#include "Platforms/OpenGL/OpenGLTexture.h"


namespace Cober {

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		: _width(width), _height(height)
	{
		_internalFormat = GL_RGBA8;
		_dataFormat = GL_RGBA;

		GLCallV(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GLCallV(glTextureStorage2D(m_RendererID, 1, _internalFormat, _width, _height));

		GLCallV(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCallV(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCallV(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCallV(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}
	

	OpenGLTexture::~OpenGLTexture()
	{
		GLCallV(glDeleteTextures(1, &m_RendererID));
	}

	
	std::string OpenGLTexture::GetName() const  {

		auto lastSlash = _path.find_last_of("/\\");
		
        LOG_CORE_ASSERT(lastSlash == std::string::npos, "Texture Path is invalid or does not exist!");
		
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = _path.rfind('.');
		
        LOG_CORE_ASSERT(lastDot == std::string::npos, "Texture Name is invalid or does not exist!");
		
		auto count = lastDot == std::string::npos ? _path.size() - lastSlash : lastDot - lastSlash;
		std::string name = _path.substr(lastSlash, count);
		return name;
	}


	std::string OpenGLTexture::GetFormat() const {

		auto lastDot = _path.rfind('.');
		std::string format = lastDot != std::string::npos ? _path.substr(lastDot) : "null";
		return format;
	}


	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = _dataFormat == GL_RGBA ? 4 : 3;
        LOG_CORE_ASSERT(size == _width * _height * bpp, "Data must be entire texture!");

		GLCallV(glTextureSubImage2D(m_RendererID, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data));
	}


	void OpenGLTexture::Bind(uint32_t slot)
	{
		GLCallV(glBindTextureUnit(slot, m_RendererID));
	}
}
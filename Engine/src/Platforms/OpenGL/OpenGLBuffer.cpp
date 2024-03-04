#include "Platforms/OpenGL/OpenGLBuffer.h"


namespace Cober {

	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]
	// [+++++++++++++++++++++++ VERTEX BUFFER ++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
	
		GLCallV(glCreateBuffers(1, &_rendererID));
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
		GLCallV(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		GLCallV(glCreateBuffers(1, &_rendererID));
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
		GLCallV(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}


	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLCallV(glDeleteBuffers(1, &_rendererID));
	}


	void OpenGLVertexBuffer::Bind() const
	{
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
	}


	void OpenGLVertexBuffer::Unbind() const
	{
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
		GLCallV(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++ INDEX BUFFER ++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: _count(count)
	{
		GLCallV(glCreateBuffers(1, &_rendererID));

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		GLCallV(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
		GLCallV(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}


	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GLCallV(glDeleteBuffers(1, &_rendererID));
	}


	void OpenGLIndexBuffer::Bind() const
	{
		GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
	}


	void OpenGLIndexBuffer::Unbind() const 
	{
		GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		GLCallV(glCreateBuffers(1, &_renderID));
		GLCallV(glNamedBufferData(_renderID, size, nullptr, GL_DYNAMIC_DRAW)); // TODO: investigate usage hint
		GLCallV(glBindBufferBase(GL_UNIFORM_BUFFER, binding, _renderID));
	}
	

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		GLCallV(glDeleteBuffers(1, &_renderID));
	}


	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		GLCallV(glNamedBufferSubData(_renderID, offset, size, data));
	}
}
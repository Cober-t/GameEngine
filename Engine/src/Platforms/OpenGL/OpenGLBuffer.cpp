#include "Platforms/OpenGL/OpenGLBuffer.h"


namespace Cober {

	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]
	// [+++++++++++++++++++++++ VERTEX BUFFER ++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) 
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		LOG_CORE_TRACE("Render VertexBuffer success");
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}


	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		LOG_CORE_TRACE("Render VertexBuffer cleaned up");
		glDeleteBuffers(1, &_rendererID);
	}


	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	}


	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++ INDEX BUFFER ++++++++++++++++++++++++++++]
	// [++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++]

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: _count(count)
	{
		glCreateBuffers(1, &_rendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		LOG_CORE_TRACE("Render IndexBuffer success");
	}


	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		LOG_CORE_TRACE("Render IndexBuffer cleaned up");
		glDeleteBuffers(1, &_rendererID);
	}


	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	}


	void OpenGLIndexBuffer::Unbind() const 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_RenderID);
		glNamedBufferData(m_RenderID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hit
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RenderID);
		LOG_CORE_TRACE("Render UniformBuffer success");
	}
	

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		LOG_CORE_TRACE("Render UniformBuffer cleaned up");
		glDeleteBuffers(1, &m_RenderID);
	}


	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RenderID, offset, size, data);
	}
}
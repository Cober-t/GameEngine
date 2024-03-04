#include "Platforms/OpenGL/OpenGLVertexArray.h"


namespace Cober {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		LOG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCallV(glCreateVertexArrays(1, &_rendererID));
	}


	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCallV(glDeleteVertexArrays(1, &_rendererID));
	}


	void OpenGLVertexArray::Bind() const
	{
		GLCallV(glBindVertexArray(_rendererID));
	}


	void OpenGLVertexArray::Unbind() const
	{
		GLCallV(glBindVertexArray(0));
	}


	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		LOG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size() != 0, "Vertex Buffer has no layout!");

		GLCallV(glBindVertexArray(_rendererID));
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					GLCallV(glEnableVertexAttribArray(_vertexBufferIndex));
					GLCallV(glVertexAttribPointer(_vertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.offset));
					_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					GLCallV(glEnableVertexAttribArray(_vertexBufferIndex));
					GLCallV(glVertexAttribIPointer(_vertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						layout.GetStride(),
						(const void*)element.offset));
					_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						GLCallV(glEnableVertexAttribArray(_vertexBufferIndex));
						GLCallV(glVertexAttribPointer(_vertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.type),
							element.normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.offset + sizeof(float) * count * i)));
						GLCallV(glVertexAttribDivisor(_vertexBufferIndex, 1));
						_vertexBufferIndex++;
					}
					break;
				}
				default: LOG_WARNING("Unknown ShaderDataType!");
			}
		}

		_vertexBuffers.push_back(vertexBuffer);
	}


	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GLCallV(glBindVertexArray(_rendererID));
		indexBuffer->Bind();

		_indexBuffer = indexBuffer;
	}
}
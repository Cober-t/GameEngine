#ifndef RENDER_API_H
#define RENDER_API_H

#include "Core/Core.h"
#include "Render/VertexArray.h"

#include <glm/glm.hpp>

namespace Cober {

	class RenderAPI 
	{

	public:
		enum class API 
		{
			None = 0, OpenGL, OpenGLES, OpenGLES3
		};
		
	public:

		virtual ~RenderAPI() = default;

		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t heigth) = 0;
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void SetClearColor(float red, float green, float blue, float black) = 0;

		virtual void SetLineWidth(float width) = 0;

		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual void ClearErrors() = 0;
		virtual void CheckErrors(const char* function) = 0;

		static API GetAPI() { return m_Api; }
		static Unique<RenderAPI> Create();
	private:
		static API m_Api;
	};
}

#endif
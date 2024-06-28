#ifndef OPENGL_RENDER_API_H
#define OPENGL_RENDER_API_H

#include "Render/RenderAPI.h"

namespace Cober {

	class OpenGLRenderAPI : public RenderAPI 
	{
	public:
		virtual void Init() override;

		virtual void Clear() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(glm::vec4 color) override;
		virtual void SetClearColor(float red, float green, float blue, float black) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		
		virtual void SetLineWidth(float width) override;

		virtual void ClearErrors() override;
		virtual void CheckErrors(const char* function) override;
	};
}

#endif
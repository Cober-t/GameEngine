#ifndef RENDER_GLOBALS_H
#define RENDER_GLOBALS_H

#include "RenderAPI.h"
#include "VertexArray.h"

namespace Cober {

	class CB_API RenderGlobals 
	{
	public:
		static void Clear();
		static void Init();

		static void SetViewport(uint32_t width, uint32_t height);
		static void SetClearColor(glm::vec4 color);
		static void SetClearColor(float red, float green, float blue, float black = 255.0f);

		static void SetLineWidth(float width);
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);

	private:
		static Unique<RenderAPI> m_Api;
	};
}

#endif
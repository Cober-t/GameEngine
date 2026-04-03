#ifndef RENDER_GLOBALS_H
#define RENDER_GLOBALS_H

#include "Core/Window.h"
#include "Render/RenderAPI.h"
#include "Render/VertexArray.h"

namespace Cober {

	class CB_API RenderGlobals 
	{
	public:
		static void Init(void* window, void* context);
		static void Clear();

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);

		static bool BeginFrame();
		static void BeginMainRenderPass();
		static void EndFrame();

        static void ImGuiInit();
        static void ImGuiShutdown();
        static void ImGuiNewFrame();
		static void ImGuiPrepareDrawData(ImDrawData* drawData);
        static void ImGuiRenderDrawData(ImDrawData* drawData);

		static void SetViewport(uint32_t width, uint32_t height);
		static void SetClearColor(glm::vec4 color);
		static void SetClearColor(float red, float green, float blue, float black = 255.0f);

		static void SetLineWidth(float width);
		static void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);


	private:
		static Unique<RenderAPI> m_Api;
	};
}

#endif
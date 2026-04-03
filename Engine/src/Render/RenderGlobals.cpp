#include <pch.h>
#include "Render/RenderGlobals.h"

namespace Cober {

	Unique<RenderAPI> RenderGlobals::m_Api = nullptr;// RenderAPI::Create();

	void RenderGlobals::Init(void* window, void* context) 
	{
		// RenderGlobals::m_Api->SetAPI(RenderAPI::API::SDLGPU);
		RenderGlobals::m_Api = RenderAPI::Create();
		if (RenderGlobals::m_Api) {
			RenderGlobals::m_Api->Init(window, context);
		}
	}

	void RenderGlobals::Clear() 
	{
		RenderGlobals::m_Api->Clear();
	}

	void RenderGlobals::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		RenderGlobals::m_Api->DrawIndexed(vertexArray, count);
	}

	bool RenderGlobals::BeginFrame() 
	{
		return RenderGlobals::m_Api->BeginFrame();
	}

	void RenderGlobals::BeginMainRenderPass() 
	{
		RenderGlobals::m_Api->BeginMainRenderPass();
	}

	void RenderGlobals::EndFrame() 
	{
		RenderGlobals::m_Api->EndFrame();
	}

    void RenderGlobals::ImGuiInit() 
	{
		RenderGlobals::m_Api->ImGuiInit();
	}

    void RenderGlobals::ImGuiShutdown() 
	{
		RenderGlobals::m_Api->ImGuiShutdown();
	}

    void RenderGlobals::ImGuiNewFrame() 
	{
		RenderGlobals::m_Api->ImGuiNewFrame();
	}

	void RenderGlobals::ImGuiPrepareDrawData(ImDrawData* drawData) 
	{
		RenderGlobals::m_Api->ImGuiPrepareDrawData(drawData);
	}

    void RenderGlobals::ImGuiRenderDrawData(ImDrawData* drawData) 
	{
		RenderGlobals::m_Api->ImGuiRenderDrawData(drawData);
	}

	void RenderGlobals::SetViewport(uint32_t width, uint32_t height) 
	{
		RenderGlobals::m_Api->SetViewport(0, 0, width, height);
	}

	void RenderGlobals::SetClearColor(glm::vec4 color) 
	{
		RenderGlobals::m_Api->SetClearColor(color);
	}

	void RenderGlobals::SetClearColor(float red, float green, float blue, float black) 
	{
		RenderGlobals::m_Api->SetClearColor(red, green, blue, black);
	}

	void RenderGlobals::DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		RenderGlobals::m_Api->DrawTriangles(vertexArray, vertexCount);
	}

	void RenderGlobals::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		RenderGlobals::m_Api->DrawLines(vertexArray, vertexCount);
	}

	void RenderGlobals::SetLineWidth(float width)
	{
		RenderGlobals::m_Api->SetLineWidth(width);
	}
}
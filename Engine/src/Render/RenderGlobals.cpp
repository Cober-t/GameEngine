#include <pch.h>
#include "RenderGlobals.h"

namespace Cober {

	Unique<RenderAPI> RenderGlobals::m_Api = RenderAPI::Create();

	void RenderGlobals::Init() 
	{
		RenderGlobals::m_Api->Init();
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


	void RenderGlobals::Clear() 
	{
		RenderGlobals::m_Api->Clear();
	}


	void RenderGlobals::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		RenderGlobals::m_Api->DrawLines(vertexArray, vertexCount);
	}


	void RenderGlobals::SetLineWidth(float width)
	{
		RenderGlobals::m_Api->SetLineWidth(width);
	}


	void RenderGlobals::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		RenderGlobals::m_Api->DrawIndexed(vertexArray, count);
	}
}
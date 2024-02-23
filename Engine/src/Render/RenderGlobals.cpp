#include "RenderGlobals.h"

namespace Cober {

	static Unique<RenderAPI> m_Api;

	void RenderGlobals::Create() 
	{
		m_Api = RenderAPI::Create();
	}

	void RenderGlobals::Init() 
	{
		if (m_Api == nullptr)
			RenderGlobals::Create();

		m_Api->Init();
	}

	void RenderGlobals::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		m_Api->SetViewport(x, y, width, height);
	}

	void RenderGlobals::SetClearColor(glm::vec4 color) 
	{
		m_Api->SetClearColor(color);
	}

	void RenderGlobals::SetClearColor(float red, float green, float blue, float black) 
	{
		m_Api->SetClearColor(red, green, blue, black);
	}

	void RenderGlobals::Clear() 
	{
		m_Api->Clear();
	}

	/*void RenderGlobals::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		m_Api->DrawIndexed(vertexArray, count);
	}*/
}
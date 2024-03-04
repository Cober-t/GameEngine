#include "RenderAPI.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"


namespace Cober {

//#ifdef __OPENGL__
	RenderAPI::API RenderAPI::m_Api = RenderAPI::API::OpenGL;
//#endif

	Unique<RenderAPI> RenderAPI::Create() {

		switch (m_Api) 
		{
			case RenderAPI::API::None:		LOG_CORE_INFO("RenderAPI::None means there is not render defined!!"); return nullptr;
			case RenderAPI::API::OpenGL:	return CreateUnique<OpenGLRenderAPI>();
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}
}
#include "RenderAPI.h"

//#if defined __EMSCRIPTEN__ || __OPENGLES3__
//#include "Platforms/OpenGLES3/OpenGLES3RenderAPI.h"
//#else
#include "Platforms/OpenGL/OpenGLRenderAPI.h"
//#endif

namespace Cober {

//#ifdef __OPENGL__
	RenderAPI::API RenderAPI::m_Api = RenderAPI::API::OpenGL;
//#elif  __OPENGLES__
//	RenderAPI::API RenderAPI::_api = RenderAPI::API::OpenGLES;
//#elif  __OPENGLES3__ || __EMSCRIPTEN__
//	RenderAPI::API RenderAPI::_api = RenderAPI::API::OpenGLES3;
//#else
//	RenderAPI::API RenderAPI::_api = RenderAPI::API::None;
//#endif

	Unique<RenderAPI> RenderAPI::Create() {

		switch (m_Api) {
		case RenderAPI::API::None:		LOG_INFO("RenderAPI::None means there is not render defined!!"); return nullptr;
		case RenderAPI::API::OpenGL:	return CreateUnique<OpenGLRenderAPI>();
		case RenderAPI::API::OpenGLES:	LOG_ERROR("Wrong API"); break;
		//case RenderAPI::API::OpenGLES3:	return CreateUnique<OpenGLES3RenderAPI>();
		default:	LOG_ERROR("Unknown RendererAPI!"); break;
		}
		LOG_INFO("Unknown RenderAPI!");
		return nullptr;
	}
}
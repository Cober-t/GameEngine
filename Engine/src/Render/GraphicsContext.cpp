#include "Core/Core.h"
#include "Render/GraphicsContext.h"

// #if !defined __OPENGLES3__
	#include "Platforms/OpenGL/OpenGLContext.h"
// #elif defiend __OPENGLES3__
// 	#include "Platforms/OpenGLES3/OpenGLES3Context.h"
// #endif


namespace Cober {

	Unique<GraphicsContext> GraphicsContext::Create(void* window)
	{
		// switch (Renderer::GetAPI())
		// {
			// case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			// case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		// }
		return CreateUnique<OpenGLContext>(static_cast<GLFWwindow*>(window));

		LOG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
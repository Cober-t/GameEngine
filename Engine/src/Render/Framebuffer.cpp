#include "Render/RenderAPI.h"
#include "Render/Framebuffer.h"

#if !defined __EMSCRIPTEN__ && !defined __OPENGLES3__
#include "Platforms/OpenGL/OpenGLFramebuffer.h"
#endif
//#include "Platforms/OpenGLES/OpenGLESFramebuffer.h"
//#include "Platforms/OpenGLES3/OpenGLES3Framebuffer.h"

namespace Cober {

	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height) 
    {
		switch (RenderAPI::GetAPI()) {
			case RenderAPI::API::None:		LOG_ERROR("RenderAPI::None means there is not render defined!!");		return nullptr;
			case RenderAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(width, height);
		}
		LOG_ERROR("Unknown Framebuffer RenderAPI!");
		return nullptr;
	}
}

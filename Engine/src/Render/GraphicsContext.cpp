#include <pch.h>
#include "Render/RenderAPI.h"
#include "Render/GraphicsContext.h"

#include "Platforms/OpenGL/OpenGLContext.h"
#include "Platforms/Vulkan/VulkanContext.h"


namespace Cober {

	Unique<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:    LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			// case RenderAPI::API::OpenGL:  return CreateUnique<OpenGLContext>(static_cast<GLFWwindow*>(window));
			case RenderAPI::API::Vulkan:  return CreateUnique<VulkanContext>(static_cast<SDL_Window*>(window));
			default: return nullptr; break;
		}
		// return CreateUnique<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}
}
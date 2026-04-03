#include <pch.h>
#include "Render/RenderAPI.h"
// #include "Platforms/Vulkan/VulkanRenderAPI.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"

namespace Cober {

	RenderAPI::API RenderAPI::m_Api = RenderAPI::API::SDLGPU;
	
	Unique<RenderAPI> RenderAPI::Create() 
	{
		switch (m_Api) 
		{
			case RenderAPI::API::None:		LOG_CORE_INFO("RenderAPI::None means there is not render defined!!"); return nullptr;
			// case RenderAPI::API::OpenGL:	return CreateUnique<OpenGLRenderAPI>();
			// case RenderAPI::API::Vulkan:	return CreateUnique<VulkanRenderAPI>();
			case RenderAPI::API::SDLGPU:	return CreateUnique<SDLGPURenderAPI>();
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}

	RenderAPI::API RenderAPI::GetAPI()
	{
		return m_Api;
	}

	void RenderAPI::SetAPI(API api)
	{
		m_Api = api;
	}
}
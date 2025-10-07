#include <pch.h>
#include "RenderAPI.h"
// #include "Platforms/OpenGL/OpenGLRenderAPI.h"
#include "Platforms/Vulkan/VulkanRenderAPI.h"

namespace Cober {

	RenderAPI::API RenderAPI::m_Api = RenderAPI::API::Vulkan;//RenderAPI::API::OpenGL;
	
	Unique<RenderAPI> RenderAPI::Create() 
	{
		switch (m_Api) 
		{
			case RenderAPI::API::None:		LOG_CORE_INFO("RenderAPI::None means there is not render defined!!"); return nullptr;
			// case RenderAPI::API::OpenGL:	return CreateUnique<OpenGLRenderAPI>();
			case RenderAPI::API::Vulkan:	return CreateUnique<VulkanRenderAPI>();
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
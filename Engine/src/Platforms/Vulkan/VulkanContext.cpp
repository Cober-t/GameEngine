#include <pch.h>
#include "Platforms/Vulkan/VulkanContext.h"

namespace Cober {

	
	VulkanContext::VulkanContext(SDL_Window* windowHandle)
		: m_windowHandle(windowHandle)
	{
		LOG_CORE_ASSERT(windowHandle, "Window handle is null!");
		LOG_CORE_INFO("Window Context Created!!");
	}

	void VulkanContext::Destroy()
	{
        SDL_DestroyGPUDevice(m_gpuDevice);
	}

	SDL_GPUDevice* VulkanContext::GetGPUDevice()
	{
		return m_gpuDevice;
	}

	void VulkanContext::Init()
	{
		m_gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL, true, NULL);
		SDL_ClaimWindowForGPUDevice(m_gpuDevice, m_windowHandle);
		// Create a 2D rendering context for a window
		//m_renderer = SDL_CreateRenderer(m_windowHandle, nullptr);

		// SDL_GetNumRenderDrivers();
		// SDL_GetRenderDriverInfo();
		const char* deviceDriver = SDL_GetGPUDeviceDriver(m_gpuDevice);
		LOG_CORE_INFO("GPU Device Driver: {0}", deviceDriver);

		// int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		// LOG_CORE_ASSERT(status, "Failed to initialize Glad!");

		// LOG_CORE_INFO("Vulkan Info:");
		// LOG_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		// LOG_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		// LOG_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

		// LOG_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), \
		// 				"The Engine requires at least OpenGL version 4.5!");
	}


	void VulkanContext::SwapBuffers()
	{
		// glfwSwapBuffers(m_WindowHandle);
	}
}
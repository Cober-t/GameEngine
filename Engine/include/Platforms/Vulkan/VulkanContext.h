#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "Render/GraphicsContext.h"

struct SDL_Window;
struct SDL_GPUDevice;
// struct SDL_Renderer;

namespace Cober {

	class VulkanContext : public GraphicsContext 
	{
	public:
		VulkanContext(SDL_Window* windowHandle);
		virtual ~VulkanContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Destroy() override;
		virtual SDL_GPUDevice* GetGPUDevice() override;

	private:
		SDL_Window*    m_windowHandle = nullptr;
		SDL_GPUDevice* m_gpuDevice = nullptr;
        // SDL_Renderer* m_renderer     = nullptr;
	};
}

#endif

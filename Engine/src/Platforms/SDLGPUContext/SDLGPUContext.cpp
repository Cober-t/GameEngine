#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace Cober 
{
    // --------------------------------------------------------------------------------------

    SDLGPUContext::SDLGPUContext(SDL_Window* windowHandle, RenderAPI::API requestedAPI)
        : m_WindowHandle(windowHandle)
    {
        LOG_CORE_ASSERT(windowHandle, "SDLGPUContext: window handle is null!");
    }

    // --------------------------------------------------------------------------------------

    void SDLGPUContext::Init()
    {
        CB_PROFILE_FUNCTION();

        LOG_CORE_ASSERT(m_WindowHandle, "SDLGPUContext: window handle is null!");

        SDL_GPUShaderFormat shaderFormatFlags =  SDL_GPU_SHADERFORMAT_SPIRV |
                                        SDL_GPU_SHADERFORMAT_DXIL | 
                                        SDL_GPU_SHADERFORMAT_MSL  |
                                        SDL_GPU_SHADERFORMAT_METALLIB;
                  
        m_GPUDevice = SDL_CreateGPUDevice(
            shaderFormatFlags, // Shader Format flags
            true, // Debug mode
            nullptr // Preferred Driver
        );

        LOG_CORE_ASSERT(m_GPUDevice, "SDL_CreateGPUDevice failed: {0}", SDL_GetError());

        const bool claimed = SDL_ClaimWindowForGPUDevice(m_GPUDevice, m_WindowHandle);
        LOG_CORE_ASSERT(claimed, "SDL_ClaimWindowForGPUDevice failed: {0}", SDL_GetError());

        const bool swapchainOk = SDL_SetGPUSwapchainParameters(
            m_GPUDevice,
            m_WindowHandle,
            SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
            SDL_GPU_PRESENTMODE_VSYNC
        );
    
        LOG_CORE_ASSERT(swapchainOk, "SDL_SetGPUSwapchainParameters failed: {0}", SDL_GetError());

        const char* actualDriver = SDL_GetGPUDeviceDriver(m_GPUDevice);
        LOG_CORE_INFO("SDL_GPU backend selected: {0}", actualDriver ? actualDriver : "unknown");

        // Other info version
        const char* backend = SDL_GetGPUDeviceDriver(m_GPUDevice);
		SDL_PropertiesID props = SDL_GetGPUDeviceProperties(m_GPUDevice);

		const char* deviceName   = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown") : "Unknown";
		const char* driverName   = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "Unknown") : "Unknown";
		const char* driverVer    = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "Unknown") : "Unknown";
		const char* driverInfo   = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, "Unknown") : "Unknown";

		LOG_CORE_INFO("SDL3 GPU Info:");
		LOG_CORE_INFO("  Backend: {0}", backend ? backend : "Unknown");
		LOG_CORE_INFO("  Device: {0}", deviceName);
		LOG_CORE_INFO("  Driver: {0}", driverName);
		LOG_CORE_INFO("  Driver Version: {0}", driverVer);
		LOG_CORE_INFO("  Driver Info: {0}", driverInfo);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPUContext::Destroy()
    {        
        if (!m_GPUDevice) {
            return;
        }

        SDL_WaitForGPUIdle(m_GPUDevice);

        if (m_WindowHandle) {
            SDL_ReleaseWindowFromGPUDevice(m_GPUDevice, m_WindowHandle);
        }

        SDL_DestroyGPUDevice(m_GPUDevice);
        m_GPUDevice = nullptr;
    }

    // --------------------------------------------------------------------------------------
}
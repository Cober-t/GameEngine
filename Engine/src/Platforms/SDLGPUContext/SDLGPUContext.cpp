#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUContext.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <imgui/imgui.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Cober {

    SDLGPUContext::SDLGPUContext(SDL_Window* windowHandle, RenderAPI::API requestedAPI)
        : m_WindowHandle(windowHandle), m_RequestedAPI(requestedAPI)
    {
        LOG_CORE_ASSERT(windowHandle, "SDLGPUContext: window handle is null!");
    }

    const char* SDLGPUContext::ToPreferredDriver(RenderAPI::API api)
    {
        switch (api)
        {
            case RenderAPI::API::Vulkan:    return "vulkan";
            case RenderAPI::API::Metal:     return "metal";
            case RenderAPI::API::DirectX12: return "direct3d12";
            case RenderAPI::API::SDLGPU:    return nullptr;
            default:                        return nullptr;
        }
    }

    Uint32 SDLGPUContext::GetShaderFormatFlags()
    {
        return SDL_GPU_SHADERFORMAT_SPIRV
             | SDL_GPU_SHADERFORMAT_DXIL
             | SDL_GPU_SHADERFORMAT_MSL
             | SDL_GPU_SHADERFORMAT_METALLIB;
    }

    void SDLGPUContext::Init()
    {
        LOG_CORE_ASSERT(m_WindowHandle, "SDLGPUContext: window handle is null!");

        const char* preferredDriver = ToPreferredDriver(m_RequestedAPI);

    #if defined(COBER_DEBUG) || defined(_DEBUG)
        constexpr bool debugMode = true;
    #else
        constexpr bool debugMode = false;
    #endif

        m_GPUDevice = SDL_CreateGPUDevice(
            static_cast<SDL_GPUShaderFormat>(GetShaderFormatFlags()),
            debugMode,
            preferredDriver
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
    }

    void SDLGPUContext::Destroy()
    {
        if (!m_GPUDevice) {
            return;
        }

        if (m_ImGuiInitialized) {
            ImGuiShutdown();
        }

        SDL_WaitForGPUIdle(m_GPUDevice);

        if (m_WindowHandle) {
            SDL_ReleaseWindowFromGPUDevice(m_GPUDevice, m_WindowHandle);
        }

        SDL_DestroyGPUDevice(m_GPUDevice);
        m_GPUDevice = nullptr;
    }

    const char* SDLGPUContext::GetBackendName() const
    {
        if (!m_GPUDevice) {
            return ToPreferredDriver(m_RequestedAPI);
        }

        return SDL_GetGPUDeviceDriver(m_GPUDevice);
    }

    void SDLGPUContext::ImGuiInit(SDL_Window* window)
    {
        LOG_CORE_ASSERT(window == m_WindowHandle, "ImGuiInit window mismatch");
        LOG_CORE_ASSERT(m_GPUDevice, "ImGuiInit called before GPU device creation");

        ImGui_ImplSDLGPU3_InitInfo initInfo = {};
        initInfo.Device = m_GPUDevice;
        initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_GPUDevice, m_WindowHandle);
        initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;

        const bool ok = ImGui_ImplSDLGPU3_Init(&initInfo);
        LOG_CORE_ASSERT(ok, "ImGui_ImplSDLGPU3_Init failed");

        m_ImGuiInitialized = true;
    }

    void SDLGPUContext::ImGuiShutdown()
    {
        if (!m_ImGuiInitialized) {
            return;
        }

        ImGui_ImplSDLGPU3_Shutdown();
        m_ImGuiInitialized = false;
    }

    void SDLGPUContext::ImGuiNewFrame()
    {
        LOG_CORE_ASSERT(m_ImGuiInitialized, "ImGuiNewFrame called before ImGuiInit");
        ImGui_ImplSDLGPU3_NewFrame();
    }

    void SDLGPUContext::ImGuiRenderDrawData(ImDrawData* drawData)
    {
        LOG_CORE_ASSERT(m_ImGuiInitialized, "ImGuiRenderDrawData called before ImGuiInit");

        if (!drawData) { 
            return;
        }
            

        if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f) {
            return;
        }

        SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(m_GPUDevice);
        LOG_CORE_ASSERT(commandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

        SDL_GPUTexture* swapchainTexture = nullptr;
        const bool acquired = SDL_WaitAndAcquireGPUSwapchainTexture(
            commandBuffer,
            m_WindowHandle,
            &swapchainTexture,
            nullptr,
            nullptr
        );

        LOG_CORE_ASSERT(acquired, "SDL_WaitAndAcquireGPUSwapchainTexture failed: {0}", SDL_GetError());

        if (!swapchainTexture)
        {
            SDL_SubmitGPUCommandBuffer(commandBuffer);
            return;
        }

        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

        SDL_GPUColorTargetInfo targetInfo = {};
        targetInfo.texture = swapchainTexture;
        targetInfo.clear_color = SDL_FColor{ 0.08f, 0.08f, 0.10f, 1.0f };
        targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        targetInfo.store_op = SDL_GPU_STOREOP_STORE;
        targetInfo.mip_level = 0;
        targetInfo.layer_or_depth_plane = 0;
        targetInfo.cycle = false;

        SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
        LOG_CORE_ASSERT(renderPass, "SDL_BeginGPURenderPass failed: {0}", SDL_GetError());

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);

        SDL_EndGPURenderPass(renderPass);

        const bool submitted = SDL_SubmitGPUCommandBuffer(commandBuffer);
        LOG_CORE_ASSERT(submitted, "SDL_SubmitGPUCommandBuffer failed: {0}", SDL_GetError());
    }

}
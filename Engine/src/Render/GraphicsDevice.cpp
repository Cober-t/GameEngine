#include <pch.h>
#include "Render/GraphicsDevice.h"
#include "Render/Framebuffer.h" // For offscreen framebuffer render passes
#include "Core/EngineApp.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>
#include <imgui/imgui.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Cober {

GraphicsDevice* GraphicsDevice::s_Instance = nullptr;

// --------------------------------------------------------------------------------------
// Initialization and Shutdown
// --------------------------------------------------------------------------------------

void GraphicsDevice::Init(SDL_Window* window)
{
    LOG_CORE_ASSERT(!s_Instance, "GraphicsDevice already initialized!");
    s_Instance = new GraphicsDevice();
    s_Instance->m_Window = window;
    LOG_CORE_ASSERT(window, "Main window is null");

    // SDL_CreateGPUDevice creates the GPU device.
    // We provide all shader format flags and let SDL3 pick the best backend
    // (Vulkan on Windows/Linux, Metal on macOS, D3D12 on Windows).
    // The debug flag (true) enables validation layers when available.
    SDL_GPUShaderFormat shaderFormatFlags = SDL_GPU_SHADERFORMAT_SPIRV |
                                            SDL_GPU_SHADERFORMAT_DXIL |
                                            SDL_GPU_SHADERFORMAT_MSL  |
                                            SDL_GPU_SHADERFORMAT_METALLIB;

    s_Instance->m_Device = SDL_CreateGPUDevice(
        shaderFormatFlags,
        true,    // Debug mode (enable validation)
        nullptr  // Preferred driver (null = auto-select)
    );
    LOG_CORE_ASSERT(s_Instance->m_Device, "SDL_CreateGPUDevice failed: {0}", SDL_GetError());

    // Claim the window for this GPU device.
    // This associates the window's swapchain with the device.
    const bool claimed = SDL_ClaimWindowForGPUDevice(s_Instance->m_Device, window);
    LOG_CORE_ASSERT(claimed, "SDL_ClaimWindowForGPUDevice failed: {0}", SDL_GetError());

    // Set swapchain parameters: SDR composition + VSYNC present mode.
    const bool swapchainOk = SDL_SetGPUSwapchainParameters(
        s_Instance->m_Device,
        window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        SDL_GPU_PRESENTMODE_VSYNC
    );
    LOG_CORE_ASSERT(swapchainOk, "SDL_SetGPUSwapchainParameters failed: {0}", SDL_GetError());

    // Log backend info
    const char* backend = SDL_GetGPUDeviceDriver(s_Instance->m_Device);
    LOG_CORE_INFO("SDL_GPU backend selected: {0}", backend ? backend : "unknown");

    SDL_PropertiesID props = SDL_GetGPUDeviceProperties(s_Instance->m_Device);

    const char* deviceName   = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown") : "Unknown";
    const char* driverName   = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "Unknown") : "Unknown";
    const char* driverVer    = props ? SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "Unknown") : "Unknown";

    LOG_CORE_INFO("  GPU: {0} | Driver: {1} v{2}", deviceName, driverName, driverVer);

    // Cache the swapchain format for pipeline creation later
    s_Instance->m_SwapchainFormat = SDL_GetGPUSwapchainTextureFormat(s_Instance->m_Device, window);

    // Initialize SDL_shadercross for runtime HLSL cross-compilation
    if (!SDL_ShaderCross_Init()) {
        LOG_CORE_WARNING("SDL_ShaderCross_Init failed: {0}", SDL_GetError());
    }
}

void GraphicsDevice::Shutdown()
{
    if (!s_Instance)
        return;

    if (s_Instance->m_ImGuiInitialized)
    {
        s_Instance->ImGuiShutdown();
    }

    if (s_Instance->m_Device)
    {
        SDL_WaitForGPUIdle(s_Instance->m_Device);

        if (s_Instance->m_Window)
            SDL_ReleaseWindowFromGPUDevice(s_Instance->m_Device, s_Instance->m_Window);

        SDL_DestroyGPUDevice(s_Instance->m_Device);
        s_Instance->m_Device = nullptr;
    }

    SDL_ShaderCross_Quit();

    delete s_Instance;
    s_Instance = nullptr;
}

GraphicsDevice& GraphicsDevice::Get()
{
    LOG_CORE_ASSERT(s_Instance, "GraphicsDevice not initialized! Call GraphicsDevice::Init() first.");
    return *s_Instance;
}

// --------------------------------------------------------------------------------------
// Frame Lifecycle
// --------------------------------------------------------------------------------------
//
// Each frame follows this sequence:
//   1. SDL_AcquireGPUCommandBuffer()      - creates a new command buffer
//   2. SDL_WaitAndAcquireGPUSwapchainTexture() - gets the texture to render into
//   3. SDL_BeginGPURenderPass()           - starts a render pass on the texture
//   4. (draw calls here)
//   5. SDL_EndGPURenderPass()             - ends the render pass
//   6. SDL_SubmitGPUCommandBuffer()       - submits work to the GPU
//
// --------------------------------------------------------------------------------------

bool GraphicsDevice::BeginFrame()
{
    // Step 1: Acquire a command buffer for this frame
    m_Frame = {};
    m_Frame.CommandBuffer = SDL_AcquireGPUCommandBuffer(m_Device);
    LOG_CORE_ASSERT(m_Frame.CommandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

    // Step 2: Wait for and acquire the swapchain texture
    // The swapchain texture is what we render into to display on screen.
    // SDL_WaitAndAcquireGPUSwapchainTexture blocks until a texture is available.
    const bool ok = SDL_WaitAndAcquireGPUSwapchainTexture(
        m_Frame.CommandBuffer,
        m_Window,
        &m_Frame.SwapchainTexture,
        nullptr,  // swapchain_available (optional)
        nullptr   // output_timestamp (optional)
    );
    LOG_CORE_ASSERT(ok, "SDL_WaitAndAcquireGPUSwapchainTexture failed: {0}", SDL_GetError());

    m_RenderingToSwapchain = false;
    return true;
}

void GraphicsDevice::EndFrame()
{
    // Step 5-6: End any active render pass, submit the command buffer
    EndActiveRenderPass();

    if (m_Frame.CommandBuffer)
    {
        SDL_SubmitGPUCommandBuffer(m_Frame.CommandBuffer);
    }

    // Release transfer buffers that were used during the frame.
    // Safe to do now because the command buffer has been submitted
    // and SDL_ReleaseGPUTransferBuffer defers the actual GPU-free until
    // the device is idle with respect to those resources.
    for (auto* buf : m_PendingTransferBuffers)
    {
        if (buf)
            SDL_ReleaseGPUTransferBuffer(m_Device, buf);
    }
    m_PendingTransferBuffers.clear();

    m_RenderingToSwapchain = false;
}

// --------------------------------------------------------------------------------------
// Render Pass Management
// --------------------------------------------------------------------------------------

void GraphicsDevice::BeginSwapchainRenderPass()
{
    // Don't start a new pass if one is already active
    if (m_Frame.RenderPass || !m_Frame.SwapchainTexture)
        return;

    // Step 3: Begin a render pass on the swapchain texture
    // The color target info specifies which texture to render into,
    // how to clear it, and what to do after rendering.
    SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
    colorTargetInfo.texture = m_Frame.SwapchainTexture;
    colorTargetInfo.clear_color = SDL_FColor{ m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;   // Clear the texture before rendering
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;  // Store the rendered result

    m_Frame.RenderPass = SDL_BeginGPURenderPass(m_Frame.CommandBuffer, &colorTargetInfo, 1, nullptr);
    LOG_CORE_ASSERT(m_Frame.RenderPass, "SDL_BeginGPURenderPass(swapchain) failed: {0}", SDL_GetError());

    m_RenderingToSwapchain = true;
    m_ActiveRenderTarget = {};
    m_ActiveRenderTarget.NumColorTargets = 1;
    m_ActiveRenderTarget.ColorFormats[0] = (int)m_SwapchainFormat;
    m_ActiveRenderTarget.HasDepth = false;
    ApplyViewport();
}

void GraphicsDevice::BeginFramebufferRenderPass(class Framebuffer* framebuffer)
{
    LOG_CORE_ASSERT(framebuffer, "Framebuffer render pass requested with null framebuffer");
    EndActiveRenderPass();

    // ---- Pre-pass: clear integer-format attachments via copy pass ----
    // SDL_GPUColorTargetInfo.clear_color is SDL_FColor (float), which can't
    // properly clear integer formats like R32_SINT. We upload -1 via a copy
    // pass before the render pass to ensure ReadPixel() gets correct values.
    for (uint32_t i = 0; i < framebuffer->GetColorAttachmentCount(); ++i)
    {
        int fmt = framebuffer->GetColorAttachmentFormat(i);
        bool isIntegerFormat =
            (fmt >= (int)SDL_GPU_TEXTUREFORMAT_R8_UINT  && fmt <= (int)SDL_GPU_TEXTUREFORMAT_R32G32B32A32_UINT) ||
            (fmt >= (int)SDL_GPU_TEXTUREFORMAT_R8_INT   && fmt <= (int)SDL_GPU_TEXTUREFORMAT_R32G32B32A32_INT);

        if (!isIntegerFormat)
            continue;

        SDL_GPUTexture* texture = framebuffer->GetColorAttachmentTexture(i);
        uint32_t w = framebuffer->GetSpecification().Width;
        uint32_t h = framebuffer->GetSpecification().Height;

        std::vector<int> clearData(w * h, -1);
        uint32_t dataSize = (uint32_t)(clearData.size() * sizeof(int));

        SDL_GPUTransferBufferCreateInfo buffInfo{};
        buffInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        buffInfo.size = dataSize;
        SDL_GPUTransferBuffer* upload = SDL_CreateGPUTransferBuffer(m_Device, &buffInfo);

        void* mapped = SDL_MapGPUTransferBuffer(m_Device, upload, false);
        SDL_memcpy(mapped, clearData.data(), dataSize);
        SDL_UnmapGPUTransferBuffer(m_Device, upload);

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(m_Frame.CommandBuffer);

        SDL_GPUTextureTransferInfo texTransferInfo{};
        texTransferInfo.transfer_buffer = upload;
        texTransferInfo.offset = 0;
        texTransferInfo.pixels_per_row = w;
        texTransferInfo.rows_per_layer = h;

        SDL_GPUTextureRegion texRegion{};
        texRegion.texture = texture;
        texRegion.mip_level = 0;
        texRegion.layer = 0;
        texRegion.x = 0;
        texRegion.y = 0;
        texRegion.z = 0;
        texRegion.w = w;
        texRegion.h = h;
        texRegion.d = 1;

        SDL_UploadToGPUTexture(copyPass, &texTransferInfo, &texRegion, false);
        SDL_EndGPUCopyPass(copyPass);
        // Defer release until after the command buffer is submitted
        m_PendingTransferBuffers.push_back(upload);
    }

    // ---- Render pass ----
    std::vector<SDL_GPUColorTargetInfo> colorInfos;
    colorInfos.reserve(framebuffer->GetColorAttachmentCount());

    for (uint32_t i = 0; i < framebuffer->GetColorAttachmentCount(); ++i)
    {
        SDL_GPUColorTargetInfo target = {};
        target.texture = framebuffer->GetColorAttachmentTexture(i);

        int fmt = framebuffer->GetColorAttachmentFormat(i);
        bool isIntegerFormat =
            (fmt >= (int)SDL_GPU_TEXTUREFORMAT_R8_UINT  && fmt <= (int)SDL_GPU_TEXTUREFORMAT_R32G32B32A32_UINT) ||
            (fmt >= (int)SDL_GPU_TEXTUREFORMAT_R8_INT   && fmt <= (int)SDL_GPU_TEXTUREFORMAT_R32G32B32A32_INT);

        if (isIntegerFormat)
        {
            target.load_op = SDL_GPU_LOADOP_LOAD;   // Already cleared via copy pass above
        }
        else
        {
            target.clear_color = (i == 0)
                ? SDL_FColor{ m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a }
                : SDL_FColor{ -1.0f, 0.0f, 0.0f, 0.0f };
            target.load_op = SDL_GPU_LOADOP_CLEAR;
        }
        target.store_op = SDL_GPU_STOREOP_STORE;
        colorInfos.push_back(target);
    }

    SDL_GPUDepthStencilTargetInfo depthTarget = {};
    depthTarget.texture = framebuffer->GetDepthAttachmentTexture();
    depthTarget.clear_depth = 1.0f;
    depthTarget.load_op = SDL_GPU_LOADOP_CLEAR;
    depthTarget.store_op = SDL_GPU_STOREOP_STORE;
    depthTarget.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
    depthTarget.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
    depthTarget.clear_stencil = 0;
    depthTarget.cycle = false;

    m_Frame.RenderPass = SDL_BeginGPURenderPass(
        m_Frame.CommandBuffer,
        colorInfos.data(),
        (uint32_t)colorInfos.size(),
        depthTarget.texture ? &depthTarget : nullptr
    );
    LOG_CORE_ASSERT(m_Frame.RenderPass, "SDL_BeginGPURenderPass(framebuffer) failed: {0}", SDL_GetError());

    m_RenderingToSwapchain = false;
    m_ActiveRenderTarget = {};
    m_ActiveRenderTarget.NumColorTargets = (uint32_t)colorInfos.size();
    for (uint32_t i = 0; i < m_ActiveRenderTarget.NumColorTargets && i < 4; ++i)
        m_ActiveRenderTarget.ColorFormats[i] = framebuffer->GetColorAttachmentFormat(i);
    m_ActiveRenderTarget.HasDepth = (depthTarget.texture != nullptr);
    m_ActiveRenderTarget.DepthFormat = framebuffer->GetDepthFormat();
    ApplyViewport();
}

void GraphicsDevice::EndActiveRenderPass()
{
    // Step 5: End the render pass if one is active
    if (m_Frame.RenderPass)
    {
        SDL_EndGPURenderPass(m_Frame.RenderPass);
        m_Frame.RenderPass = nullptr;
    }
}

// --------------------------------------------------------------------------------------
// State Setters
// --------------------------------------------------------------------------------------

void GraphicsDevice::SetClearColor(glm::vec4 color)
{
    // Auto-detect 0-255 range and normalize to 0-1 if needed
    if (color.r > 1.0f && color.g > 1.0f && color.b > 1.0f && color.a > 1.0f)
        color /= glm::vec4(255.0f);
    m_ClearColor = color;
}

void GraphicsDevice::SetClearColor(float r, float g, float b, float a)
{
    SetClearColor(glm::vec4(r, g, b, a));
}

void GraphicsDevice::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    m_Viewport = { x, y, width, height };
    ApplyViewport();
}

void GraphicsDevice::ApplyViewport()
{
    if (!m_Frame.RenderPass || m_Viewport.z == 0 || m_Viewport.w == 0)
        return;

    SDL_GPUViewport viewport{};
    viewport.x = (float)m_Viewport.x;
    viewport.y = (float)m_Viewport.y;
    viewport.w = (float)m_Viewport.z;
    viewport.h = (float)m_Viewport.w;
    viewport.min_depth = 0.0f;
    viewport.max_depth = 1.0f;

    SDL_SetGPUViewport(m_Frame.RenderPass, &viewport);
}

void GraphicsDevice::UpdatePassSignatureForSwapchain()
{
    // (reserved for future pipeline cache key usage)
}

// --------------------------------------------------------------------------------------
// ImGui Integration
// --------------------------------------------------------------------------------------

void GraphicsDevice::ImGuiInit()
{
    LOG_CORE_ASSERT(m_Window, "ImGuiInit called before window creation");
    LOG_CORE_ASSERT(m_Device, "ImGuiInit called before GPU device creation");

    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.ColorTargetFormat = m_SwapchainFormat;
    initInfo.Device = m_Device;
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;

    const bool ok = ImGui_ImplSDLGPU3_Init(&initInfo);
    LOG_CORE_ASSERT(ok, "ImGui_ImplSDLGPU3_Init failed");

    m_ImGuiInitialized = true;
}

void GraphicsDevice::ImGuiShutdown()
{
    if (!m_ImGuiInitialized)
        return;

    ImGui_ImplSDLGPU3_Shutdown();
    m_ImGuiInitialized = false;
}

void GraphicsDevice::ImGuiNewFrame()
{
    LOG_CORE_ASSERT(m_ImGuiInitialized, "ImGuiNewFrame called before ImGuiInit");
    ImGui_ImplSDLGPU3_NewFrame();
}

void GraphicsDevice::ImGuiPrepareDrawData(ImDrawData* drawData)
{
    if (!m_ImGuiInitialized || !drawData || !m_Frame.CommandBuffer)
        return;

    ImGui_ImplSDLGPU3_PrepareDrawData(drawData, m_Frame.CommandBuffer);
}

void GraphicsDevice::ImGuiRenderDrawData(ImDrawData* drawData)
{
    if (!m_ImGuiInitialized || !drawData)
        return;

    // Ensure we have a render pass to draw into
    BeginSwapchainRenderPass();
    if (!m_Frame.RenderPass)
        return;

    ImGui_ImplSDLGPU3_RenderDrawData(drawData, m_Frame.CommandBuffer, m_Frame.RenderPass);
}

} // namespace Cober

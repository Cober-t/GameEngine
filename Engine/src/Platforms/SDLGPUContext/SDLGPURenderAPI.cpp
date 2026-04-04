#include <pch.h>
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"
#include "Core/EngineApp.h"

#include <SDL3/SDL_gpu.h>

#include <imgui/imgui.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Cober 
{
	// void OpenGLMessageCallback(
	// 	unsigned source,
	// 	unsigned type,
	// 	unsigned id,
	// 	unsigned severity,
	// 	int length,
	// 	const char* message,
	// 	const void* userParam)
	// {
	// 	switch (severity)
	// 	{
	// 		case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_CRITICAL(message); return;
	// 		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
	// 		case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARNING(message); return;
	// 		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
	// 	}
		
	// 	LOG_CORE_ASSERT(false, "Unknown severity level!");
	// }
    
    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::Init(void* window, void* context) 
	{
        CB_PROFILE_FUNCTION();
        
        m_windowHandle = static_cast<SDL_Window*>(window);
        LOG_CORE_ASSERT(m_windowHandle, "Main Window does not exists");

        m_GPUDevice = static_cast<SDL_GPUDevice*>(context);
        // LOG_CORE_ASSERT(m_GPUDevice, "Window GPU Device does not exists");

        m_frame = new SDLGPUFrame();

		LOG_CORE_TRACE("Render API init (Graphics Pipeline Render API)");
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		// glViewport(0, 0, width, height);
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::SetClearColor(glm::vec4 color) 
	{
		m_clearColor = { color.x, color.y, color.z, color.a };
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::SetClearColor(float red, float green, float blue, float black) 
	{
		m_clearColor = { red, green , blue, black };
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::Clear() 
	{

	}

    // --------------------------------------------------------------------------------------

    bool SDLGPURenderAPI::BeginFrame()
    {
        CB_PROFILE_FUNCTION();

        // m_frame->RenderPass = nullptr;
        m_frame->CommandBuffer = SDL_AcquireGPUCommandBuffer(m_GPUDevice);
        LOG_CORE_ASSERT(m_frame->CommandBuffer, "Command Buffer couldn't be acquired");

        const bool ok = SDL_WaitAndAcquireGPUSwapchainTexture(
            m_frame->CommandBuffer,
            m_windowHandle,
            &m_frame->SwapchainTexture,
            nullptr,
            nullptr
        );
        LOG_CORE_ASSERT(ok, "Swapchain acquire failed");

        return true;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::BeginMainRenderPass() 
    {
        CB_PROFILE_FUNCTION();

        if (EngineApp::Get().IsMinimized()) {
            return;
        }
        // LOG_CORE_ASSERT(m_frame->CommandBuffer, "No command buffer");

        SDL_GPUColorTargetInfo targetInfo = {};
        targetInfo.texture = m_frame->SwapchainTexture;
        targetInfo.clear_color = SDL_FColor{ m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a };
        targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        targetInfo.store_op = SDL_GPU_STOREOP_STORE;

        m_frame->RenderPass = SDL_BeginGPURenderPass(m_frame->CommandBuffer, &targetInfo, 1, nullptr);
        LOG_CORE_ASSERT(m_frame->RenderPass, "RenderPass couldn't be acquired");
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::EndFrame() 
    {
        CB_PROFILE_FUNCTION();

        if (m_frame->RenderPass && !EngineApp::Get().IsMinimized()) {
            SDL_EndGPURenderPass(m_frame->RenderPass);
        }

        if (m_frame->CommandBuffer) {
            SDL_SubmitGPUCommandBuffer(m_frame->CommandBuffer);
        }

        // m_frame->RenderPass = nullptr;
        // m_frame->SwapchainTexture = nullptr;
        // m_frame->CommandBuffer = nullptr;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiInit()
    {
        CB_PROFILE_FUNCTION();

        LOG_CORE_ASSERT(m_windowHandle, "ImGuiInit called before Window creation");
        LOG_CORE_ASSERT(m_GPUDevice, "ImGuiInit called before GPU device creation");

        ImGui_ImplSDLGPU3_InitInfo initInfo = {};
        initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_GPUDevice, m_windowHandle);
        initInfo.Device = m_GPUDevice;
        initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;

        const bool ok = ImGui_ImplSDLGPU3_Init(&initInfo);
        LOG_CORE_ASSERT(ok, "ImGui_ImplSDLGPU3_Init failed");

        m_ImGuiInitialized = true;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiShutdown()
    {
        CB_PROFILE_FUNCTION();

        if (!m_ImGuiInitialized) {
            return;
        }

        ImGui_ImplSDLGPU3_Shutdown();
        m_ImGuiInitialized = false;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiNewFrame()
    {
        CB_PROFILE_FUNCTION();

        LOG_CORE_ASSERT(m_ImGuiInitialized, "ImGuiNewFrame called before ImGuiInit");
        ImGui_ImplSDLGPU3_NewFrame();
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiPrepareDrawData(ImDrawData* drawData)
    {
        CB_PROFILE_FUNCTION();

        if (EngineApp::Get().IsMinimized()) {
            return;
        }

        LOG_CORE_ASSERT(drawData, "ImGUI no draw data available!");
        if (!m_frame->SwapchainTexture && EngineApp::Get().IsMinimized()) {
            return;
        }

        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, m_frame->CommandBuffer);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPURenderAPI::ImGuiRenderDrawData(ImDrawData* drawData)
    {
        CB_PROFILE_FUNCTION();

        if (!drawData || !m_frame->CommandBuffer || !m_frame->RenderPass)
        return;

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, m_frame->CommandBuffer, m_frame->RenderPass);
    }

    // --------------------------------------------------------------------------------------
    
	void SDLGPURenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
        CB_PROFILE_FUNCTION();
	// 	vertexArray->Bind();
	// 	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	// 	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	
    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		// vertexArray->Bind();
		// glDrawArrays(GL_LINES, 0, vertexCount);
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		// vertexArray->Bind();
		// glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::SetLineWidth(float width)
	{
		// glLineWidth(width);
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::ClearErrors() 
	{
		// while (glGetError());
	}

    // --------------------------------------------------------------------------------------

	void SDLGPURenderAPI::CheckErrors(const char* function) 
	{
		// while (GLenum error = glGetError()) {
		// 	std::string fileName = (std::string)__FILE__;
		// 	//std::string solutionDir = SOLUTION_DIR;
		// 	//fileName = fileName.substr(solutionDir.length());

		// 	fileName = fileName.substr(fileName.find_last_of("\\") + 1);
		// 	std::string errMessage = (const char*)glGetString(error);
		// 	printf("%s", errMessage.c_str());
		// 	LOG_CORE_ASSERT(false, "[OpenGL Error] (" + errMessage + ") " + function + " " + fileName);
		// }
	}

    // --------------------------------------------------------------------------------------
}
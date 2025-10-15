#include <pch.h>
#include "Platforms/Vulkan/VulkanRenderAPI.h"
#include "EngineApp.h"

namespace Cober {

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


	void VulkanRenderAPI::Init() 
	{
		/// TODO: Maybe create GraphicsPipelineRenderAPI (SDL3)
	// #ifdef CB_DEBUG
		// glEnable(GL_DEBUG_OUTPUT);
		// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	// #endif

		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// glEnable(GL_DEPTH_TEST);

		// glEnable(GL_LINE_SMOOTH);

		LOG_CORE_TRACE("Render API init (Graphics Pipeline Render API)");
	}


	void VulkanRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		// glViewport(0, 0, width, height);
	}


	void VulkanRenderAPI::SetClearColor(glm::vec4 color) 
	{
		m_clearColor = { color.x, color.y, color.z, color.a };
	}


	void VulkanRenderAPI::SetClearColor(float red, float green, float blue, float black) 
	{
		m_clearColor = { red, green , blue, black };
	}


	void VulkanRenderAPI::Clear() 
	{
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		EngineApp& app = EngineApp::Get();
		SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app.GetWindow().GetContext()->GetGPUDevice());
		if (cmdbuf == NULL) {
			SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
			return ;
		}

		SDL_GPUTexture* swapchainTexture;
		if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, app.GetWindow().GetRawWindow(), &swapchainTexture, NULL, NULL)) {
			SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
			return;
		}

		if (swapchainTexture != NULL)
		{
			SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
			colorTargetInfo.texture = swapchainTexture;
			colorTargetInfo.clear_color = { m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3] };
			colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

			SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
			SDL_EndGPURenderPass(renderPass);
		}

		SDL_SubmitGPUCommandBuffer(cmdbuf);
	}


	void VulkanRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
	// 	vertexArray->Bind();
	// 	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	// 	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	

	void VulkanRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		// vertexArray->Bind();
		// glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void VulkanRenderAPI::DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		// vertexArray->Bind();
		// glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void VulkanRenderAPI::SetLineWidth(float width)
	{
		// glLineWidth(width);
	}


	void VulkanRenderAPI::ClearErrors() 
	{
		// while (glGetError());
	}


	void VulkanRenderAPI::CheckErrors(const char* function) 
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
}
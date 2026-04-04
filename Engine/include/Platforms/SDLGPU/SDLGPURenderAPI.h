#ifndef SDLGPU_RENDER_API_H
#define SDLGPU_RENDER_API_H

#include "Render/RenderAPI.h"

struct SDL_Window;
struct SDL_GPUDevice;
struct ImDrawData;

/**
 * @class SDLGRPURenderApi
 * @brief A class that performs specific calculations.
 * 
 * This class provides ....
 */
namespace Cober {
    
    class VertexArray;

 	class SDLGPURenderAPI : public RenderAPI 
 	{
 	public:
		/**
		 * @brief Init the render framework
		 * @param ..
		 * @return ..
		 */
		virtual void Init(void* window, void* context) override;

		virtual void Clear() override;
	
		virtual bool BeginFrame() override;
		virtual void BeginMainRenderPass() override;
		virtual void EndFrame() override;
		
        // Dear ImGui renderer-backend hooks
        virtual void ImGuiInit() override;
        virtual void ImGuiShutdown() override;
        virtual void ImGuiNewFrame() override;
        virtual void ImGuiPrepareDrawData(ImDrawData* drawData) override;
        virtual void ImGuiRenderDrawData(ImDrawData* drawData) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(glm::vec4 color) override;
		virtual void SetClearColor(float red, float green, float blue, float black) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		
		virtual void SetLineWidth(float width) override;

		virtual void ClearErrors() override;
		virtual void CheckErrors(const char* function) override;

	private:
		struct SDLGPUFrame
        {
            SDL_GPUCommandBuffer* CommandBuffer = nullptr;
            SDL_GPUTexture* SwapchainTexture = nullptr;
            SDL_GPURenderPass* RenderPass = nullptr;
        };
        SDLGPUFrame* m_frame = {};
		SDL_Window* m_windowHandle = nullptr;
		SDL_GPUDevice* m_GPUDevice = nullptr;
		
        bool m_ImGuiInitialized = false;
		glm::vec4 m_clearColor = {1.0f, 0.0f, 0.0f, 1.0f};
 	};
}

#endif
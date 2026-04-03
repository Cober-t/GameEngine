// #ifndef VULKAN_RENDER_API_H
// #define VULKAN_RENDER_API_H

// #include "Render/RenderAPI.h"

// struct VkDevice;
// struct VkCommandBuffer;
// struct VkPipeline;

// namespace Cober {
    
//     class VertexArray;

//  	class VulkanRenderAPI : public RenderAPI 
//  	{
//  	public:
// 		virtual void Init(void* window, void* context) override {};

// 		virtual void Clear() override {};

// 		virtual void* GetDevice() {};
//         virtual void* GetWindowHandle() {};
		
// 		virtual bool BeginFrame() override {};
// 		virtual void EndFrame() override {};

//         // Dear ImGui renderer-backend hooks
//         virtual void ImGuiInit() override {};
//         virtual void ImGuiShutdown() override {};
//         virtual void ImGuiNewFrame() override {};
//         virtual void ImGuiRenderDrawData(ImDrawData* drawData) override {};
		
// 		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {};
// 		inline virtual void SetClearColor(glm::vec4 color) override {
// 			m_clearColor = { color.x, color.y, color.z, color.a };
// 		};
// 		inline virtual void SetClearColor(float red, float green, float blue, float black) override {
// 			m_clearColor = { red, green , blue, black };
// 		};

// 		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override {};
// 		virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override {};
// 		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override {};
		
// 		virtual void SetLineWidth(float width) override {};

// 		virtual void ClearErrors() override {};
// 		virtual void CheckErrors(const char* function) override {};

// 	private:
// 		std::vector<float> m_clearColor = {1.0f, 0.0f, 0.0f, 1.0f};
// 		struct VulkanFrame
//         {
//             VkCommandBuffer* CommandBuffer = nullptr;
// 			VkPipeline* Pipeline = nullptr;
//         };

//         VulkanFrame* m_vkFrame = {};
//         SDL_Window* m_SDLWindowHandle = nullptr;
//         VkDevice* m_vkDevice = nullptr;
//         bool m_ImGuiInitialized = false;
//  	};
// }

// #endif
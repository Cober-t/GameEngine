#ifndef QUAD_H
#define QUAD_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

class Render2D;

namespace Cober  {

	namespace Primitive {

		class Quad
		{
		public:
			static void Init();
			static void InitFramebuffer();
			static void CleanVertexBuffer();

			static void Draw(Entity& entity);
			// static void Draw(const glm::mat4& transform, const glm::vec4& color);
			static void Draw(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture>& subTexture = nullptr, int entityID = -1);
			static void Draw(const std::vector<glm::vec3> vertices, int vertexCount, const glm::vec4& color);
			static void DrawRect(Entity& entity);
			static void DrawRect(const glm::vec3& position, const glm::vec3& rotation, uint32_t width, uint32_t height, const glm::vec4& color, int entityID = -1);
			// static void DrawRect(glm::vec3 position, glm::vec3 rotation, float width, float height, Entity& entityID, glm::vec4 color = glm::vec4(1.0f));
			static void DrawTexture(Entity& entity);
			static void DrawFramebuffer(const Ref<Framebuffer>& framebuffer);

			static void Flush();
			static void StartBatch();
			static void NextBatch();
			static void EndBatch();

		public:
			struct Attributes
			{
				glm::vec3 Position;
				glm::vec4 Color;
				glm::vec2 TexCoord;
				float TexIndex;
				float TilingFactor;

				// Editor-only
				int EntityID;
			};

			struct Data
			{
				Ref<VertexArray> FramebufferVAO;
				Ref<VertexBuffer> FramebufferVBO;
				Ref<Shader> FramebufferShader;
				Ref<Texture> FramebufferTexture;

				Ref<VertexArray> VertexArray;
				Ref<VertexBuffer> VertexBuffer;
				Ref<Shader> Shader;
				Ref<Texture> QuadTexture;

				uint32_t IndexCount = 0;

				static const uint32_t MaxTextureSlots = 32;
				std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
				uint32_t TextureSlotIndex = 1; // 0 = white texture
				
				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};
		};
	}
}

#endif
#ifndef QUAD_H
#define QUAD_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

class Render2D;

namespace Cober {

	namespace Primitive {

		class Quad
		{
		public:
			static void Init();
			static void CleanVertexBuffer();

			static void Draw(Entity& entity);
			static void Draw(const std::vector<glm::vec3> vertices, int vertexCount, const glm::vec4& color);
			static void DrawRect(Entity& entity);
			// static void DrawRect(glm::vec3 position, glm::vec3 rotation, float width, float height, Entity& entityID, glm::vec4 color = glm::vec4(1.0f));
			static void DrawTexture(Entity& entity);

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
				Ref<VertexArray> VertexArray;
				Ref<VertexBuffer> VertexBuffer;
				Ref<Shader> Shader;
				Ref<Texture> WhiteTexture;

				uint32_t IndexCount = 0;

				static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
				std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
				uint32_t TextureSlotIndex = 1; // 0 = white texture
				
				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};

		private:
			static void SetAttributes(const glm::mat4& transform, const glm::vec4& color, float textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);
		};
	}
}

#endif
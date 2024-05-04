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
			Quad();
			virtual ~Quad();
			
			static void Init();

			static void Draw(Entity& entity);
			static void DrawRect(Entity& entity);

			static void Flush();
			static void StartBatch();
			static void NextBatch();
			static void EndBatch();
			static uint32_t GetIndexCount();

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
			static Quad* s_Instance;
			static void SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);
		};
	}
}

#endif
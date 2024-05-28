#ifndef CIRCLE_H
#define CIRCLE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Text/Font.h"
#include "Scene/ECS.h"

class Render2D;

namespace Cober  {

	namespace Primitive {

		class Text
		{
		public:
			static void Init();
			static void CleanVertexBuffer();

			static void Draw(Entity& entity);
			static void Draw(const glm::mat4& transform, const glm::vec4& color, float thickness);
			
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

                // TODO: bg color for outline/bg

                int EntityID;
			};

			struct Data
			{
				Ref<VertexArray> VertexArray;
				Ref<VertexBuffer> VertexBuffer;
				Ref<Shader> Shader;
				
                Ref<Texture> FontAtlasTexture;
                Ref<Texture> WhiteTexture;
                static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
				std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
				uint32_t TextureSlotIndex = 1;
				uint32_t IndexCount = 0;

				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};

			Attributes* attributes;

		private:
			static void SetAttributes(const glm::mat3& position, const glm::vec4& color, const glm::vec2& texCoord, int entityID);
		};
	}
}


#endif
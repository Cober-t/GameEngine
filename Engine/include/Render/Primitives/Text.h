#ifndef TEXT_H
#define TEXT_H

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
			
			static void Flush();
			static void StartBatch();
			static void NextBatch();
			static void EndBatch();
			static uint32_t GetIndexCount();

		public:
			struct TextParams
			{
				glm::vec4 Color{ 1.0f };
				float Kerning = 0.0f;
				float LineSpacing = 0.0f;
			};

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
				uint32_t IndexCount = 0;

				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};

			Attributes* attributes;

		private:
			static void SetAttributes(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID);
		};
	}
}


#endif
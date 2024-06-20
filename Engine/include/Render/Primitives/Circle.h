#ifndef CIRCLE_H
#define CIRCLE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

class Render2D;

namespace Cober  {

	namespace Primitive {

		class Circle
		{
		public:
			static void Init();
			static void CleanVertexBuffer();

			static void Draw(Entity& entity);
			static void Draw(const glm::mat4& transform, const glm::vec4& color, float thickness, int entityID = -1);
			
			static void Flush();
			static void StartBatch();
			static void NextBatch();
			static void EndBatch();
			static uint32_t GetIndexCount();

		public:
			struct Attributes
			{
				glm::vec3 WorldPosition;
				glm::vec3 LocalPosition;
				glm::vec4 Color;
				float Thickness;
				float Fade;

				// Editor-only
				int EntityID;
			};

			struct Data
			{
				Ref<VertexArray> VertexArray;
				Ref<VertexBuffer> VertexBuffer;
				Ref<Shader> Shader;
				
				uint32_t IndexCount = 0;

				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};

			Attributes* attributes;

		private:
			static void Circle::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID);
		};
	}
}


#endif
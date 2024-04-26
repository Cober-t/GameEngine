#ifndef CIRCLE_H
#define CIRCLE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"


namespace Cober  {

	namespace Primitive {

		class Circle
		{
		public:
			Circle();
			~Circle();

			static void Init();

			static void Draw(Entity& entity);
			
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

				static const uint32_t MaxCircles = 20000;
				static const uint32_t MaxVertices = MaxCircles * 4;
				static const uint32_t MaxIndices = MaxCircles * 6;

				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;

				glm::vec4 VertexPositions[4];
				size_t VertexCount = 4;
			};

			Attributes* attributes;

		private:
			static Circle* s_Instance;
			static void Circle::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID);
		};
	}
}


#endif
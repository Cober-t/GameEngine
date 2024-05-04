#ifndef LINE_H
#define LINE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

class Render2D;

namespace Cober  {

	namespace Primitive {

		class Line
		{
		public:
			Line();
			~Line();

			static void Init();

			static void Draw(Entity& entity);
			static void Draw(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID);

			static void Flush();
			static void StartBatch();
			static void NextBatch();
			static void EndBatch();
			static uint32_t GetVertexCount();

		public:
			struct Attributes
			{
				glm::vec3 Position;
				glm::vec4 Color;

				int EntityID;
			};

			struct Data
			{
				Ref<VertexArray> VertexArray;
				Ref<VertexBuffer> VertexBuffer;
				Ref<Shader> Shader;
				
				uint32_t VertexCount = 0;
				Attributes* VertexBufferBase = nullptr;
				Attributes* VertexBufferPtr = nullptr;
			};

		private:
			static Line* s_Instance;
			static void SetAttributes(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		};
	}
}


#endif
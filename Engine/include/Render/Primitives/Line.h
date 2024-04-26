#ifndef LINE_H
#define LINE_H

#include "Render/Primitives/Primitive.h"
#include "Scene/ECS.h"

namespace Cober  {

    class Line : public Primitive
    {
	public:
		Line();
		virtual ~Line() override;

		virtual void Init() override;

		virtual void Draw(Entity& entity);
		
		virtual void Flush() override;
		virtual void StartBatch() override;
		virtual void NextBatch() override;
		virtual void EndBatch() override;
		uint32_t GetVertexCount() const;

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
			
			static const uint32_t MaxLines = 100000;
			static const uint32_t MaxVertices = MaxLines * 2; // Add Lines, Circles and Cubes to the count

			float thickness = 2.0f;
		};

	private:
		void SetAttributes(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
	};
}


#endif
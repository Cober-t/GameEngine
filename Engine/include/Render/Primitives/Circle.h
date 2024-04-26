#ifndef CIRCLE_H
#define CIRCLE_H

#include "Render/Primitives/Primitive.h"
#include "Scene/ECS.h"


namespace Cober  {

	class Render2D;

    class Circle : public Primitive
    {
	public:
		Circle();
		virtual ~Circle() override;

		virtual void Init() override;

		void Draw(Entity& entity);
		
		virtual void Flush() override;
		virtual void StartBatch() override;
		virtual void NextBatch() override;
		virtual void EndBatch() override;
		uint32_t GetIndexCount() const;


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
		void Circle::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID);
	};
}


#endif
#ifndef LINE_H
#define LINE_H

#include "Render/Primitives/Primitive.h"

namespace Cober  {

    class Line : public Primitive
    {
	public:
		Line();
		virtual ~Line() override;

		virtual void Init() override;

		void Draw();
		
		virtual void Flush() override;
		virtual void StartBatch() override;
		virtual void NextBatch() override;
		virtual void EndBatch() override;

	private:
		void SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);

	private:
		struct Attributes
		{
		};

		struct Data
		{
		};

		Attributes* attributes;
	};
}


#endif
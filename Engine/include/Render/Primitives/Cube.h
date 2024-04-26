#ifndef CUBE_H
#define CUBE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

namespace Cober  {

    class Cube
    {
	public:
		Cube();
		~Cube();

		static void Init();

		static void Draw(Entity& entity);
		
		static void Flush();
		static void StartBatch();
		static void NextBatch();
		static void EndBatch();


	public:
		struct Attributes
		{
		};

		struct Data
		{
		};

		Attributes* attributes;
	
	private:
		static Cube* s_Instance;
		static void SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);
	};
}


#endif
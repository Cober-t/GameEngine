#ifndef RENDER_2D_H
#define RENDER_2D_H

#include <array>

#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera/Camera.h"

#include "Scene/ECS.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cober {

	class Render2D 
	{
	public:
		static void Start();

		static void BeginScene(const Ref<Camera>& camera);
		static void EndScene();

		static void Shutdown();

		// [++++++ BATCH +++++++]
		static void Flush();
		static void StartBatch();
		static void NextBatch();

		// EXPORT TO DEBUG RENDERER
		static void DrawSolidPolygon();
		static void DrawSolidPolygon(Entity& entity);

	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount()  { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

		// Grid data. Must be always float because WebGL dont work with ints
		// Later in the shader we can convert to ints
	};
}

#endif
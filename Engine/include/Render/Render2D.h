#ifndef RENDER_2D_H
#define RENDER_2D_H

#include "Core/EngineApp.h"

#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera/Camera.h"
#include "Render/RenderGlobals.h"

#include "Physics/Debug2DPhysics.h"

#include "Scene/ECS.h"
#include "Core/Settings.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cober {

	class CB_API Render2D 
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

		static void DrawQuad(Entity& entity);
		static void DrawRect(glm::vec3 position, glm::vec3 rotation, float width, float height, Entity& entityID, glm::vec4 color = glm::vec4(1.0f));
		static void DrawLine(Entity& entity);
		static void DrawCircle(Entity& entity);
		static void DrawSprite(Entity& entity);
		static void DrawText(Entity& entity);

	public:
		static void ResetStats();
		static RenderSettings& GetStats();

	private:
		static RenderSettings* m_RenderSettings;
	};
}

#endif
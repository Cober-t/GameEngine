#ifndef RENDER_2D_H
#define RENDER_2D_H

#include "Core/EngineApp.h"

#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera/Camera.h"
#include "Render/RenderGlobals.h"
#include "Render/Framebuffer.h"

#include "Physics/Debug2DPhysics.h"

#include "Scene/ECS.h"
#include "Core/EngineSettings.h"

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
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture>& subTexture = nullptr, int entityID = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec3& rotation, uint32_t width, uint32_t height, const glm::vec4& color, int entityID = -1);
		static void DrawLine(Entity& entity);
		static void DrawCircle(Entity& entity);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, int entityID);

		static void DrawFramebuffer(const Ref<Framebuffer>& framebuffer);

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
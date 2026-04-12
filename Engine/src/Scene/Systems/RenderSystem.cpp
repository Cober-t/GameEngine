#include <pch.h>
#include "Scene/Systems/RenderSystem.h"
#include "Physics/Physics2D.h"
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Scene/Systems/CameraSystem.h"

namespace Cober 
{
	// --------------------------------------------------------------------------------------

	RenderSystem::RenderSystem()
    {
		LOG_INFO("Render System Added to Registry!!");
	}

	// --------------------------------------------------------------------------------------

	RenderSystem::~RenderSystem() 
    {
		LOG_INFO("Render System Removed from Registry");
	}

	// --------------------------------------------------------------------------------------

	void RenderSystem::Start()
	{
		LOG_INFO("Render System Started!!");
	}

	// --------------------------------------------------------------------------------------

	void RenderSystem::Update(Scene* scene)
	{	
		// Render primitives
		auto renderGroup = scene->GetAllEntitiesWith<Render2DComponent>();
		for (auto& entityHandle : renderGroup)
        {
			Entity entity(entityHandle, scene);
			
			switch (entity.GetComponent<Render2DComponent>().shapeType)
			{
				case Shape2D::Line:   Render2D::DrawLine(entity);   break;
				case Shape2D::Quad:   Render2D::DrawQuad(entity);   break;
				case Shape2D::Circle: Render2D::DrawCircle(entity); break;
				case Shape2D::Sprite: Render2D::DrawSprite(entity); break;
			}
		}

#if 0
		// Render Text
		auto textGroup = scene->GetAllEntitiesWith<TransformComponent, TagComponent, TextComponent>();
		for (auto& entityHandle : textGroup)
        {
			Entity entity(entityHandle, scene);
			Render2D::DrawText(entity);
		}

		Physics2D::DebugDraw(scene);
#endif
	}

	// --------------------------------------------------------------------------------------
}
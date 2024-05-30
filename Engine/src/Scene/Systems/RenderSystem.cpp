#include <pch.h>
#include "Scene/Systems/RenderSystem.h"
#include "Physics/Physics2D.h"
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Scene/Systems/CameraSystem.h"

namespace Cober {

	RenderSystem::RenderSystem()
    {
		LOG_INFO("Render System Added to Registry!!");
	}

	RenderSystem::~RenderSystem() 
    {
		LOG_INFO("Render System Removed from Registry");
	}


	void RenderSystem::Start()
	{
		LOG_INFO("Render System Started!!");
	}


	void RenderSystem::Update(Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, Render2DComponent>();

		for (auto& entt : view)
        {
			Entity entity = Entity((entt::entity)entt, scene );
			
			switch (entity.GetComponent<Render2DComponent>().shapeType)
			{
			case Shape2D::Line:
				Render2D::DrawLine(entity);
				break;
			case Shape2D::Quad:
	            Render2D::DrawQuad(entity);
				break;
			case Shape2D::Circle:
				Render2D::DrawCircle(entity);
				break;
			case Shape2D::Sprite:
				Render2D::DrawSprite(entity);
				break;
			}
		}

		auto viewText = scene->GetAllEntitiesWith<TransformComponent, TagComponent, TextComponent>();
		for (auto& entt : viewText)
        {
			Entity entity = Entity((entt::entity)entt, scene );
			Render2D::DrawText(entity);
		}

		if (EngineApp::Get().GetGameState() == EngineApp::GameState::RUNTIME_EDITOR)
			Physics2D::DebugDraw();
	}
}
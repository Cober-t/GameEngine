#include <pch.h>
#include "Scene/Systems/RenderSystem.h"
#include "Scene/Systems/PhysicsSystem2D.h"

namespace Cober {

	RenderSystem::RenderSystem(Scene* scene) : m_Scene(scene)
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


	void RenderSystem::Update(Unique<Timestep>& ts, const Ref<Camera>& camera)
	{
		Render2D::ResetStats();
		Render2D::BeginScene(camera);

		for (auto entity : GetSystemEntities())
        {
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
				break;
			}

			// In the future... if (EngineApp::Get().IsDebugMode() && entity.GetComponent<DebugComponent>.isActive)
			if (EngineApp::Get().IsDebugMode())
			{
				switch (entity.GetComponent<Render2DComponent>().shapeType)
				{
				case Shape2D::Quad:
					if (Debug2DPhysics::Settings().QUADS)
						Render2D::DebugDrawQuad(entity);
					break;
					
				case Shape2D::Circle:
					if (Debug2DPhysics::Settings().CIRCLES)
						Render2D::DebugDrawCircle(entity);
					break;
				}

				if (Debug2DPhysics::Settings().TRANSFORMS)
					Render2D::DebugDrawTransform(entity);
			}
		}

		Render2D::EndScene();
	}


	std::vector<Entity> RenderSystem::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, TagComponent, Render2DComponent>();

		for (auto entity : entitiesView)
		{
			entities.emplace_back( Entity((entt::entity)entity, m_Scene ));
		}

		return entities;
	};
}
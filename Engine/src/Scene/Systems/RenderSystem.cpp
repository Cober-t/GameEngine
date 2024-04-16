#include <pch.h>
#include "Scene/Systems/RenderSystem.h"

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
		RenderGlobals::Init();
		Render2D::Start();

		LOG_INFO("Render System Started!!");
	}


	void RenderSystem::Update(Unique<Timestep>& ts, const Ref<Camera>& camera)
	{
		camera->OnUpdate(ts);
		Render2D::ResetStats();
		Render2D::BeginScene(camera);
		

		for (auto entity : GetSystemEntities())
        {
            Render2D::DrawSolidQuad(entity);
		}

		Render2D::EndScene();
	}


	std::vector<Entity> RenderSystem::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, TagComponent>();
		// auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, SpriteComponent>();

		for (auto entity : entitiesView)
		{
			entities.emplace_back( Entity((entt::entity)entity, m_Scene ));
		}

		return entities;
	};
}
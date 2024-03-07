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


	void RenderSystem::Update(Timestep ts, const Ref<GameCamera>& camera)
	{
        camera->OnUpdate(ts);

		Render2D::ResetStats();
		Render2D::BeginScene(camera);
		RenderGlobals::SetClearColor(10, 0, 10, 255);

		for (auto& entity : GetSystemEntities())
        {
            Render2D::DrawSolidPolygon(entity);
		}

		RenderGlobals::Clear();
		Render2D::EndScene();
	}


	std::vector<Entity> RenderSystem::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, TagComponent>();
		// auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, SpriteComponent>();

		for (auto entity : entitiesView)
		{
			Entity newEntity = { entity, m_Scene };
			// entities.emplace( newEntity );
			entities.emplace_back( newEntity );
		}

		return entities;
	};
}
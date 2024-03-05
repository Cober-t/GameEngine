#include "Scene/Systems/RenderSystem.h"


namespace Cober {

	RenderSystem::RenderSystem() 
    {
		RequireComponent<TransformComponent>();

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
			TransformComponent transform = entity.GetComponent<TransformComponent>();
		
            Render2D::DrawSolidPolygon();
		}

		RenderGlobals::Clear();
		Render2D::EndScene();
	}
}
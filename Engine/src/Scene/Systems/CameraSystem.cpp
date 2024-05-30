#include <pch.h>
#include "Scene/Systems/CameraSystem.h"

namespace Cober {

	CameraSystem::CameraSystem()
    {
		LOG_INFO("Camera System Added to Registry!!");
	}

	CameraSystem::~CameraSystem() 
    {
        // auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, CameraComponent>();
        // if (view != entt::null)
        // {
        //     for (auto& entt : view)
        //     {
        //         Entity entity = Entity((entt::entity)entt, scene );
        //         entity.GetComponent<CameraComponent>().camera = nullptr;
        //     }
        // }
		LOG_INFO("Camera System Removed from Registry");
	}


	void CameraSystem::Start(const Scene* scene)
	{
        // auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, CameraComponent>();
        // if (view != entt::null)
        // {
        //     for (auto& entt : view)
        //     {
        //         Entity entity = Entity((entt::entity)entt, scene );
        //         entity.GetComponent<CameraComponent>().camera = nullptr;
        //     }
        // }
		LOG_INFO("Camera System Started!!");
	}


	void CameraSystem::Update(Unique<Timestep>& ts, Ref<Camera>& defaultCamera, Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, CameraComponent>();

        if (view.size_hint() > 0)
        {
            for (auto& entt : view)
            {
                Entity entity = Entity((entt::entity)entt, scene );

                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& transformComponent = entity.GetComponent<TransformComponent>();
                auto& gameCamera = cameraComponent.camera;

                if (gameCamera && cameraComponent.mainCamera)
                {
                    auto& gameCameraSettings = cameraComponent.camera->GetSettings();
                    
                    // Assignment must be done only one time in the future
                    cameraComponent.SetMain(true);
                    defaultCamera->SetMainCamera(false);

                    gameCameraSettings.focalPoint = transformComponent.position;
                    gameCameraSettings.yaw = transformComponent.rotation.y;
                    gameCameraSettings.pitch = -transformComponent.rotation.x;
                    gameCameraSettings.roll = transformComponent.rotation.z;

                    // Handle a list of cameras in the future to avoid return
                    defaultCamera = cameraComponent.camera;
                    cameraComponent.camera->OnUpdate(ts);

                    // if (cameraComponent.debug)
                        // Render2D::DrawQuad()

                    return;
                }
            }
        }

        // If the future list of cameras is not empty render the default camera
        defaultCamera->SetMainCamera(true);
        defaultCamera->OnUpdate(ts);
	}


    // void CameraSystem::UpdateSimulation(Unique<Timestep>& ts, const Ref<Camera>& defaultCamera, Scene* scene)
    // {
    //     defaultCamera->OnUpdate(ts);
    // }
}
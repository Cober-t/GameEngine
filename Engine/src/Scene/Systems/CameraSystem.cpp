#include <pch.h>
#include "Scene/Systems/CameraSystem.h"

namespace Cober {

	CameraSystem::CameraSystem()
    {
		LOG_INFO("Camera System Added to Registry!!");
	}

	CameraSystem::~CameraSystem() 
    {
		LOG_INFO("Camera System Removed from Registry");
	}


	void CameraSystem::Start(const Scene* scene)
	{
		LOG_INFO("Camera System Started!!");
	}


	void CameraSystem::Update(Unique<Timestep>& ts, Ref<Camera>& defaultCamera, Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, CameraComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& transformComponent = entity.GetComponent<TransformComponent>();
            auto& gameCamera = cameraComponent.camera;

            // if (cameraComponent.debug)
            // {
            //     Render2D::DrawRect(transformComponent.position, transformComponent.rotation,
            //                         cameraComponent.width, cameraComponent.height, entity);
            // }

            if (gameCamera && cameraComponent.mainCamera)
            {
                auto& cameraSettings = gameCamera->GetSettings();
                
                cameraSettings.focalPoint = transformComponent.position;
                cameraSettings.yaw = transformComponent.rotation.y;
                cameraSettings.pitch = -transformComponent.rotation.x;
                cameraSettings.roll = transformComponent.rotation.z;

                // Handle a list of cameras in the future to avoid return
                defaultCamera = cameraComponent.camera;
                cameraComponent.camera->OnUpdate(ts);

                return;
            }
        }

        // If the future list of cameras is not empty render the default camera
        defaultCamera->OnUpdate(ts);
	}
}
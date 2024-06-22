#include <pch.h>
#include "Render/Camera/GlobalCamera.h"
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


	void CameraSystem::Start(Scene* scene)
	{
        float screenWidth = EngineApp::Get().GetWindow().GetWidth();
        float screenHeight = EngineApp::Get().GetWindow().GetHeight();
        GlobalCamera::camera = CreateUnique<EditorCamera>(45.0f, screenWidth, screenHeight, 0.01f, 1000.0f, GlobalCamera::perspective);

        auto view = scene->GetAllEntitiesWith<TransformComponent, TagComponent, CameraComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& transformComponent = entity.GetComponent<TransformComponent>();
            auto& gameCamera = cameraComponent.camera;

            if (gameCamera && cameraComponent.mainCamera)
            {
                cameraComponent.camera;
                cameraComponent.camera->SetPerspective(cameraComponent.perspective);
                cameraComponent.camera->SetMainCamera();
                cameraComponent.camera->SetViewportSize(screenWidth, screenHeight);

                // TODO: Handle a list of cameras in the future to avoid return
                return;
            }

        }
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
            auto& gameCamera = defaultCamera;

            if (gameCamera == nullptr)
                continue;

            // if (cameraComponent.debug)
            // {
            //     Render2D::DrawRect();
            // }

            if (cameraComponent.mainCamera)
            {
                auto& cameraSettings = gameCamera->GetSettings();

                cameraSettings.focalPoint = transformComponent.position;
                cameraSettings.yaw = transformComponent.rotation.y;
                cameraSettings.pitch = -transformComponent.rotation.x;
                cameraSettings.roll = transformComponent.rotation.z;

                // TODO: Handle a list of cameras in the future to avoid return
                defaultCamera = cameraComponent.camera;
                defaultCamera->OnUpdate(ts);

                return;
            }
        }

        if (defaultCamera->IsMainCamera() == false)
        {
            defaultCamera->SetMainCamera();
        }

        if(defaultCamera->IsPerspective() != GlobalCamera::perspective)
        {   
            defaultCamera->SetPerspective(GlobalCamera::perspective);
        }

        defaultCamera->OnUpdate(ts);
	}


    void CameraSystem::OnEvent(Event& event, const Ref<Camera>& camera)
    {
        camera->OnEvent(event);
    }
}

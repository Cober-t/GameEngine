#include <pch.h>
#include "Render/Camera/GameCamera.h"
#include "Core/EngineApp.h"
#include "Core/Input.h"

#include "Render/RenderGlobals.h"
#include "Events/ApplicationEvents.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Cober {

	GameCamera::GameCamera(float fov, float width, float height, float nearClip, float farClip, bool persp)
		: Camera(glm::perspectiveFov(glm::radians(fov), width, height, farClip, nearClip), glm::perspectiveFov(glm::radians(fov), width, height, nearClip, farClip))
	{
		auto& m_GameCamera = GetSettings();
		m_GameCamera.fov = fov;
		m_GameCamera.width = width;
		m_GameCamera.height = height;

		m_GameCamera.focalPoint = glm::vec3(0.0f, 0.0f, -1.0f);
		m_GameCamera.nearClip = nearClip;
		m_GameCamera.farClip = farClip;
		m_GameCamera.perspectiveProjection = persp;

		m_GameCamera.distance = glm::distance(m_GameCamera.position, m_GameCamera.focalPoint);

		m_GameCamera.yaw = 0.0f;
		m_GameCamera.pitch = 0.0f;
		m_GameCamera.roll = 0.0f;

		m_GameCamera.position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_GameCamera.direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_GameCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
		
		SetViewportSize(width, height);

		LOG_INFO("Game Camera Created!!");
	}


	GameCamera::~GameCamera()
	{
		LOG_INFO("Game Camera Destroyed!!");
	}


	void GameCamera::UpdateCameraView()
	{
		auto& m_GameCamera = GetSettings();
		m_GameCamera.position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_GameCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(m_ViewMatrix));
	}


	void GameCamera::SetViewportSize(float width, float height)
	{
		if (IsMainCamera() == false)
			return;

		auto& m_GameCamera = GetSettings();
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	
		if (IsPerspective())
		{
			SetPerspectiveProjectionMatrix(glm::radians(m_GameCamera.fov), 
										  m_ViewportWidth, m_ViewportHeight, 
										  m_GameCamera.nearClip, m_GameCamera.farClip);
		}
		else
		{
			float srcAspectRatio = m_GameCamera.width / m_GameCamera.height;
			float dstAspectRatio = width / height;
			float newWidth = m_GameCamera.width*0.01/2.0f;
			float newHeight = m_GameCamera.height*0.01/2.0f;

			// Check if the viewport is wider or taller
			if (dstAspectRatio >= srcAspectRatio) 
			{
				SetOrthoProjectionMatrix(dstAspectRatio/srcAspectRatio * newWidth, newHeight,
										m_GameCamera.nearClip, m_GameCamera.farClip);
			}
			else 
			{
				SetOrthoProjectionMatrix(newWidth, srcAspectRatio/dstAspectRatio * newHeight,
										m_GameCamera.nearClip, m_GameCamera.farClip);
			}
		}

		UpdateCameraView();
		m_GameCamera.aspectRatio = width / height;

		// In the Editor mode the game camera is managed by the viewporIn Play mode the viewport is manage by the camera
        // In the rest, the viewport is managed by the Editor Viewport
        if (EngineApp::Get().GetGameState() == EngineApp::GameState::PLAY)
        {
			RenderGlobals::SetViewport(m_ViewportWidth, m_ViewportHeight);
        }
	}


	void GameCamera::SetPerspective(bool persp)
	{
		GetSettings().perspectiveProjection = persp;
		UpdateCameraView();
	}


	glm::vec3 GameCamera::GetUpDirection() const 
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0));
	}


	glm::vec3 GameCamera::GetRightDirection() const
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0));
	}


	glm::vec3 GameCamera::GetForwardDirection() const
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0));
	}


	glm::vec3 GameCamera::CalculatePosition() const 
	{
		return GetSettings().focalPoint - GetForwardDirection() * GetSettings().distance;
	}


	glm::quat GameCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-GetSettings().pitch, -GetSettings().yaw, 0.0f));
	}

	
	void GameCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		if (IsMainCamera() == false)
			return;

		UpdateCameraView();
	}

	
	void GameCamera::OnEvent(Event& event) 
	{
		if (IsMainCamera() == false)
			return;

		if (event.GetEventType() == EventType::WindowResize)
		{
			float screenWidth = static_cast<WindowResizeEvent&>(event).GetWidth();
			float screenHeight = static_cast<WindowResizeEvent&>(event).GetHeight();
			SetViewportSize(screenWidth, screenHeight);
		}

		if (Input::IsKeyDown(KeyCode::LeftControl) && Input::IsKeyPressed(KeyCode::F))
		{
			EngineApp::Get().GetWindow().ChangeFullScreen();
		}


		// Test
		if (Input::IsKeyDown(KeyCode::Up))
			GetSettings().distance -= 0.2;
		if (Input::IsKeyDown(KeyCode::Down))
			GetSettings().distance += 0.2;

		if (Input::IsKeyDown(KeyCode::LeftControl) && Input::IsKeyPressed(KeyCode::P))
		{
			float screenWidth = EngineApp::Get().GetWindow().GetWidth();
			float screenHeight = EngineApp::Get().GetWindow().GetHeight();
			float aspectRatio = screenWidth / screenHeight;
			LOG_WARNING("SWidth {0} - SHeight {1} - Ratio {2} - Persp {3}", screenWidth, screenHeight, aspectRatio, IsPerspective());
			// LOG_WARNING("CamWidth {0} - CamHeight {1} - Ratio {2} - Persp {3}", GetSettings().width, GetSettings().height, 
			// 	GetSettings().width / GetSettings().height, GetSettings().perspectiveProjection);
		}
	}
}
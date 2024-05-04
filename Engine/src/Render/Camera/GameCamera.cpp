#include <pch.h>
#include "Render/Camera/GameCamera.h"

#include "Render/RenderGlobals.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Cober {

	GameCamera::GameCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho)
		: Camera(glm::perspectiveFov(glm::radians(fov), width, height, farClip, nearClip), glm::perspectiveFov(glm::radians(fov), width, height, nearClip, farClip)),
		m_GameCamera(CameraSettings(fov, width, height, nearClip, farClip, ortho))
	{
		m_GameCamera.focalPoint = glm::vec3(0.0f, 0.0f, -1.0f);
		m_GameCamera.nearClip = nearClip;
		m_GameCamera.farClip = farClip;

		m_GameCamera.distance = glm::distance(m_GameCamera.position, m_GameCamera.focalPoint);

		m_GameCamera.yaw = 0.0f;
		m_GameCamera.pitch = 0.0f;
		m_GameCamera.roll = 0.0f;
		// m_GameCamera.yaw = 3.0f * glm::pi<float>() / 4.0f;
		// m_GameCamera.pitch = glm::pi<float>() / 4.0f;
		// m_GameCamera.roll = 0.0f;

		m_GameCamera.position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_GameCamera.direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_GameCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
		
		SetViewportSize(width, height);

		LOG_INFO("Editor Camera Created!!");
	}


	GameCamera::~GameCamera()
	{
		LOG_INFO("Game Camera Destroyed!!");
	}


	void GameCamera::UpdateCameraView()
	{
		m_GameCamera.position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_GameCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(m_ViewMatrix));
	}


	void GameCamera::SetViewportSize(float width, float height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
				return;

		SetPerspectiveProjectionMatrix(glm::radians(m_GameCamera.fov), (float)width, (float)height, m_GameCamera.nearClip, m_GameCamera.farClip);
		m_ViewportWidth = width;
		m_ViewportHeight = height;
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
		return m_GameCamera.focalPoint - GetForwardDirection() * m_GameCamera.distance;
	}


	glm::quat GameCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_GameCamera.pitch, -m_GameCamera.yaw, 0.0f));
	}

	
	void GameCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		// m_GameCamera.position = CalculatePosition();
		UpdateCameraView();
	}

	
	void GameCamera::OnEvent(Event& event) 
	{
	
	}
}
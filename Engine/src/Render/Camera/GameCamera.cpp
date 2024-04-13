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
		m_GameCamera.focalPoint = glm::vec3(0.0f);
		m_GameCamera.verticalFov = glm::radians(fov);
		m_GameCamera.nearClip = nearClip;
		m_GameCamera.farClip = farClip;
		
		// glm::vec3 position = { -5, 5, 5 };
		m_GameCamera.distance = glm::distance(m_GameCamera.position, m_GameCamera.focalPoint);

		m_GameCamera.yaw = 3.0f * glm::pi<float>() / 4.0f;
		m_GameCamera.pitch = glm::pi<float>() / 4.0f;

		m_GameCamera.position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_GameCamera.direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_GameCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
		
		LOG_INFO("Game Camera Created!!");
	}


	GameCamera::~GameCamera()
	{
		LOG_INFO("Game Camera Destroyed!!");
	}


	void GameCamera::UpdateCameraView()
	{
		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		// Extra step to handle the problem when the camera direction is the same as the up vector
		const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
		if (cosAngle * yawSign > 0.99f)
			m_GameCamera.pitchDelta = 0.0f;

		const glm::vec3 lookAt = m_GameCamera.position + GetForwardDirection();
		m_GameCamera.direction = glm::normalize(lookAt - m_GameCamera.position);
		m_GameCamera.distance = glm::distance(m_GameCamera.position, m_GameCamera.focalPoint);
		glm::mat4 viewMatrix = glm::lookAt(m_GameCamera.position, lookAt, glm::vec3{ 0.0f, yawSign, 0.0f });
		SetViewMatrix(viewMatrix);

		//damping for smooth camera
		m_GameCamera.yawDelta *= 0.6f;
		m_GameCamera.pitchDelta *= 0.6f;
		m_GameCamera.positionDelta *= 0.8f;
	}


	void GameCamera::SetViewportSize(float width, float height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
				return;

		SetPerspectiveProjectionMatrix(m_GameCamera.verticalFov, (float)width, (float)height, m_GameCamera.nearClip, m_GameCamera.farClip);
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
		return m_GameCamera.focalPoint - GetForwardDirection() * m_GameCamera.distance + m_GameCamera.positionDelta;
	}


	glm::quat GameCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_GameCamera.pitch - m_GameCamera.pitchDelta, -m_GameCamera.yaw - m_GameCamera.yawDelta, 0.0f));
	}

	
	void GameCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		m_GameCamera.position = CalculatePosition();
		UpdateCameraView();
	}

	
	void GameCamera::OnEvent(Event& event) 
	{
	
	}
}
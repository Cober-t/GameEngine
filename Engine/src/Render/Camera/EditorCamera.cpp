#include "Render/Camera/EditorCamera.h"

#include "Render/RenderGlobals.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cober {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip, bool ortho)
		: m_OrthoProjection(ortho)
	{
		if (m_OrthoProjection)
			m_OrthoCamera = EditorOrthoCamera(fov, aspectRatio, nearClip, farClip);
		else
			m_PerspCamera = EditorPerspCamera(fov, aspectRatio, nearClip, farClip);

		SetViewportSize(1280, 720);

		LOG_INFO("Editor Camera Created!!");
	}


	EditorCamera::~EditorCamera()
	{
		LOG_INFO("Editor Camera Destroyed!!");
	}


	void EditorCamera::UpdateProjection(bool& ortho) 
	{
		m_OrthoProjection = ortho;

		if (ortho) 
		{
			m_OrthoCamera.aspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_Projection = glm::ortho(-m_OrthoCamera.aspectRatio * m_OrthoCamera.distance, 	// Left
									   m_OrthoCamera.aspectRatio * m_OrthoCamera.distance,	// Right
									  -m_OrthoCamera.distance, m_OrthoCamera.distance, 		// Bottom & Top
									   m_OrthoCamera.nearClip, m_OrthoCamera.farClip);		// Near & Far
		}
		else 
		{
			m_PerspCamera.aspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_Projection = glm::perspective(glm::radians(m_PerspCamera.fov),
										   m_PerspCamera.aspectRatio, 
										   m_PerspCamera.nearClip, 
										   m_PerspCamera.farClip);
		}
	}


	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection(m_OrthoProjection);
	}


	void EditorCamera::UpdateView() 
	{
		if (m_OrthoProjection) 
		{
			m_OrthoCamera.position = CalculatePosition();
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_OrthoCamera.position) * glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 0, 1));
			m_ViewMatrix = glm::inverse(transform);
		}
		else 
		{
			// _yaw = _pitch = 0.0f;	// Lock the camera's rotation
			m_PerspCamera.position = CalculatePosition();
			glm::quat orientation = GetOrientation();
			m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_PerspCamera.position) * glm::toMat4(orientation);
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
		}
	}

	
	std::pair<float, float> EditorCamera::PanSpeed() const 
	{
		float x = std::min(m_ViewportWidth  / 1000.0f, 2.4f); // max = 2.4f
		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f

		float xFactor = 0.05f * (x * x) - 0.1778f * x + 0.3021f;
		float yFactor = 0.05f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}


	float EditorCamera::ZoomSpeed() const 
	{
		float currentDistance = m_OrthoProjection == true ? m_OrthoCamera.distance : m_PerspCamera.distance;
		float distance = currentDistance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);	// max speed = 100

		return speed;
	}


	glm::quat EditorCamera::GetOrientation() const
	{
		float newPitch = m_OrthoProjection ? m_OrthoCamera.pitch : m_PerspCamera.pitch;
		float newYaw   = m_OrthoProjection ? m_OrthoCamera.yaw   : m_PerspCamera.yaw;
		float newRoll  = m_OrthoProjection ? m_OrthoCamera.roll  : m_PerspCamera.roll;

		return glm::quat(glm::vec3(-newPitch, -newYaw, newRoll));
	}

	
	glm::vec3 EditorCamera::GetUpDirection() const 
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0));
	}


	glm::vec3 EditorCamera::GetRightDirection() const
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0));
	}


	glm::vec3 EditorCamera::GetForwardDirection() const
	{ 
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0));
	}


	glm::vec3 EditorCamera::CalculatePosition() const 
	{
		return m_OrthoProjection ? m_OrthoCamera.focalPoint - GetForwardDirection() * m_OrthoCamera.distance
								 : m_PerspCamera.focalPoint - GetForwardDirection() * m_PerspCamera.distance;
	}


	void EditorCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		UpdateView();
	}

	
	void EditorCamera::OnEvent(Event& event) 
	{
	
	}
}
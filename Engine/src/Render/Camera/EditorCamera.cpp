#include <pch.h>
#include "Render/Camera/EditorCamera.h"
#include "Core/EngineApp.h"

#include "Render/RenderGlobals.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/detail/type_quat.hpp>


namespace Cober {

	EditorCamera::EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho)
		: Camera(glm::perspectiveFov(glm::radians(fov), width, height, farClip, nearClip), glm::perspectiveFov(glm::radians(fov), width, height, nearClip, farClip)),
		m_EditorCamera(CameraSettings(fov, width, height, nearClip, farClip, ortho))
	{
		m_EditorCamera.focalPoint = glm::vec3(0.0f, 0.0f, -1.0f);
		m_EditorCamera.nearClip = nearClip;
		m_EditorCamera.farClip = farClip;

		m_EditorCamera.distance = glm::distance(m_EditorCamera.position, m_EditorCamera.focalPoint);

		m_EditorCamera.yaw = 3.0f * glm::pi<float>() / 4.0f;
		m_EditorCamera.pitch = glm::pi<float>() / 4.0f;
		m_EditorCamera.roll = 0.0f;

		m_EditorCamera.position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_EditorCamera.direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_EditorCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
		
		SetViewportSize(width, height);

		LOG_INFO("Editor Camera Created!!");
	}


	EditorCamera::~EditorCamera()
	{
		LOG_INFO("Editor Camera Destroyed!!");
	}


	void EditorCamera::SetViewportSize(float width, float height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
				return;
		SetPerspectiveProjectionMatrix(glm::radians(m_EditorCamera.fov), (float)width, (float)height, m_EditorCamera.nearClip, m_EditorCamera.farClip);
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// In the future, to change between ortho and perspective
		// UpdateCameraView();
	}


	void EditorCamera::UpdateCameraView() 
	{
		m_EditorCamera.position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_EditorCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(m_ViewMatrix));
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
		float distance = m_EditorCamera.distance * 0.2f;
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);	// max speed = 100

		return speed;
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.3f;
	}


	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
	{
		MouseZoom(event.GetYOffset() * 0.1f);
		UpdateCameraView();

		return true;
	}


	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		std::cout << "ENTER" << std::endl;
		auto [xSpeed, ySpeed] = PanSpeed();
		m_EditorCamera.focalPoint -= GetRightDirection() * delta.x * xSpeed * m_EditorCamera.distance;
		m_EditorCamera.focalPoint += GetUpDirection() * delta.y * ySpeed * m_EditorCamera.distance;
	}


	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		m_EditorCamera.yaw += yawSign * delta.x * RotationSpeed();
		m_EditorCamera.pitch += delta.y * RotationSpeed();
	}


	void EditorCamera::MouseZoom(float delta)
	{
		m_EditorCamera.distance -= delta * ZoomSpeed();

		if (m_EditorCamera.distance < 1.0f)
		{
			m_EditorCamera.focalPoint += GetForwardDirection();
			m_EditorCamera.distance = 1.0f;
		}
	}
	

	float EditorCamera::GetCameraSpeed() const
	{
		float speed = m_NormalSpeed;
		if (Input::IsKeyDown(KeyCode::LeftControl))
			speed /= 2 - glm::log(m_NormalSpeed);
		if (Input::IsKeyDown(KeyCode::LeftShift))
			speed *= 2 - glm::log(m_NormalSpeed);

		return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
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
		return m_EditorCamera.focalPoint - GetForwardDirection() * m_EditorCamera.distance;
	}


	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_EditorCamera.pitch - m_EditorCamera.pitchDelta, -m_EditorCamera.yaw, -m_EditorCamera.roll));
	}


	static void DisableMouse()
	{
		Input::SetCursorMode(CursorMode::Locked);
		// EngineApp::Get().GetImGuiLayer()->SetInputEnabled(false);
	}

	static void EnableMouse()
	{
		Input::SetCursorMode(CursorMode::Normal);
		// EngineApp::Get().GetImGuiLayer()->SetInputEnabled(true);
	}

	void EditorCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;

		if (!m_IsActive)
		{
			// if (!EngineApp::Get().GetImGuiLayer()->IsInputEnabled())
			// 	EngineApp::Get().GetImGuiLayer()->SetInputEnabled(true);

			return;
		}

		if (Input::IsKeyDown(KeyCode::LeftAlt))
		{
			if (Input::IsMouseButtonDown(MouseButton::Middle))
			{
				DisableMouse();
				MousePan(delta);
			}
			else if (Input::IsMouseButtonDown(MouseButton::Left))
			{
				DisableMouse();
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonDown(MouseButton::Right))
			{
				DisableMouse();
				MouseZoom(delta.y);
			}
			else
				EnableMouse();
		}
		else
		{
			EnableMouse();
		}

		m_InitialMousePosition = mouse;

		UpdateCameraView();
	}

	
	void EditorCamera::OnEvent(Event& event) 
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScroll(e); });
	}
}
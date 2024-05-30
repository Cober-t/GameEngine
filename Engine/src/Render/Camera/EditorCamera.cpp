#include <pch.h>
#include "Render/Camera/EditorCamera.h"
#include "Core/EngineApp.h"

#include "Render/RenderGlobals.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/detail/type_quat.hpp>


namespace Cober {

	EditorCamera::EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool persp)
		: Camera(glm::perspectiveFov(glm::radians(fov), width, height, farClip, nearClip), glm::perspectiveFov(glm::radians(fov), width, height, nearClip, farClip))
	{
		auto& m_EditorCamera = GetSettings();
		m_EditorCamera.fov = fov;
		m_EditorCamera.width = width;
		m_EditorCamera.height = height;

		m_EditorCamera.focalPoint = glm::vec3(0.0f, 0.0f, -1.0f);
		m_EditorCamera.nearClip = nearClip;
		m_EditorCamera.farClip = farClip;
		m_EditorCamera.perspectiveProjection = persp;

		m_EditorCamera.distance = glm::distance(m_EditorCamera.position, m_EditorCamera.focalPoint);

		m_EditorCamera.yaw = 0.0f;
		m_EditorCamera.pitch = 0.0;
		m_EditorCamera.roll = 0.0f;
		// m_EditorCamera.yaw = 3.0f * glm::pi<float>() / 4.0f;
		// m_EditorCamera.pitch = glm::pi<float>() / 4.0f;
		// m_EditorCamera.roll = 0.0f;

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
		if (IsMainCamera() == false)
			return;

		auto& m_EditorCamera = GetSettings();
		if (IsPerspective())
		{
			SetPerspectiveProjectionMatrix(glm::radians(m_EditorCamera.fov), (float)width, (float)height, m_EditorCamera.nearClip, m_EditorCamera.farClip);
		}
		else
		{
			m_EditorCamera.aspectRatio = (float)width / (float)height;
			SetOrthoProjectionMatrix(m_EditorCamera.aspectRatio * m_EditorCamera.distance, // Left
									 m_EditorCamera.aspectRatio * m_EditorCamera.distance,// Right
									 m_EditorCamera.distance,	// Bottom 
									 m_EditorCamera.distance,	// Top
									 m_EditorCamera.nearClip, m_EditorCamera.farClip);
		}
		
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateCameraView();
	}


	void EditorCamera::UpdateCameraView() 
	{
		auto& m_EditorCamera = GetSettings();
		m_EditorCamera.position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_EditorCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
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
		float distance = GetSettings().distance * 0.2f;
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);	// max speed = 100

		return speed;
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.3f;
	}


	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		auto& m_EditorCamera = GetSettings();

		m_EditorCamera.focalPoint -= GetRightDirection() * delta.x * xSpeed * m_EditorCamera.distance;
		m_EditorCamera.focalPoint += GetUpDirection() * delta.y * ySpeed * m_EditorCamera.distance;
	}


	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		auto& m_EditorCamera = GetSettings();

		m_EditorCamera.yaw += yawSign * delta.x * RotationSpeed();
		m_EditorCamera.pitch += delta.y * RotationSpeed();
	}


	void EditorCamera::MouseZoom(float delta)
	{
		// Becasue the distance is involve and needs to be updated
		auto& m_EditorCamera = GetSettings();
		if (IsPerspective() == false)
		{
			SetOrthoProjectionMatrix(m_EditorCamera.aspectRatio * m_EditorCamera.distance, // Left
									 m_EditorCamera.aspectRatio * m_EditorCamera.distance,// Right
									 m_EditorCamera.distance,	// Bottom 
									 m_EditorCamera.distance,	// Top
									 m_EditorCamera.nearClip, m_EditorCamera.farClip);
		}
		
		m_EditorCamera.distance -= delta * ZoomSpeed();

		if (m_EditorCamera.distance < 1.0f)
		{
			m_EditorCamera.focalPoint += GetForwardDirection();
			m_EditorCamera.distance = 1.0f;
		}
		UpdateCameraView();
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
		return GetSettings().focalPoint - GetForwardDirection() * GetSettings().distance;
	}


	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-GetSettings().pitch, -GetSettings().yaw, -GetSettings().roll));
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

		if (m_IsActive == false || IsMainCamera() == false)
		{
			// if (!EngineApp::Get().GetImGuiLayer()->IsInputEnabled())
			// 	EngineApp::Get().GetImGuiLayer()->SetInputEnabled(true);

			return;
		}

		if (Input::IsKeyDown(KeyCode::LeftAlt))
		{
			// PERSP Controls
			if (IsPerspective() == true)
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
				else
					EnableMouse();
			}
			// ORTHO Controls
			else if (IsPerspective() == false)
			{
				if (Input::IsMouseButtonDown(MouseButton::Left))
				{
					DisableMouse();
					MousePan(delta);
				}
				else
					EnableMouse();
			}
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
		if (IsMainCamera() == false)
			return;

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScroll(e); });
	}


	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
	{
		MouseZoom(event.GetYOffset() * 0.1f);
		UpdateCameraView();
		return true;
	}
}
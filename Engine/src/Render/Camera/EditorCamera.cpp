#include "Render/Camera/EditorCamera.h"

#include "Render/RenderGlobals.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/detail/type_quat.hpp>


namespace Cober {

	EditorCamera::EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho)
		: Camera(glm::perspectiveFov(glm::radians(fov), width, height, farClip, nearClip), glm::perspectiveFov(glm::radians(fov), width, height, nearClip, farClip)),
		m_EditorCamera(CameraSettings(fov, width, height, nearClip, farClip, ortho))
	{

		m_EditorCamera.focalPoint = glm::vec3(0.0f);
		m_EditorCamera.verticalFov = glm::radians(fov);
		m_EditorCamera.nearClip = nearClip;
		m_EditorCamera.farClip = farClip;

		m_EditorCamera.distance = glm::distance(m_EditorCamera.position, m_EditorCamera.focalPoint);

		m_EditorCamera.yaw = 3.0f * glm::pi<float>() / 4.0f;
		m_EditorCamera.pitch = glm::pi<float>() / 4.0f;

		m_EditorCamera.position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_EditorCamera.direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_EditorCamera.position) * glm::toMat4(orientation);
		SetViewMatrix(glm::inverse(viewMatrix));
		
		SetViewportSize(1280, 720);

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
		SetPerspectiveProjectionMatrix(m_EditorCamera.verticalFov, (float)width, (float)height, m_EditorCamera.nearClip, m_EditorCamera.farClip);
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}


	void EditorCamera::UpdateCameraView() 
	{
		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		// Extra step to handle the problem when the camera direction is the same as the up vector
		const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
		if (cosAngle * yawSign > 0.99f)
			m_EditorCamera.pitchDelta = 0.f;

		const glm::vec3 lookAt = m_EditorCamera.position + GetForwardDirection();
		m_EditorCamera.direction = glm::normalize(lookAt - m_EditorCamera.position);
		m_EditorCamera.distance = glm::distance(m_EditorCamera.position, m_EditorCamera.focalPoint);
		glm::mat4 viewMatrix = glm::lookAt(m_EditorCamera.position, lookAt, glm::vec3{ 0.0f, yawSign, 0.0f });
		SetViewMatrix(viewMatrix);

		//damping for smooth camera
		m_EditorCamera.yawDelta *= 0.6f;
		m_EditorCamera.pitchDelta *= 0.6f;
		m_EditorCamera.positionDelta *= 0.8f;
	}


	void EditorCamera::Focus(const glm::vec3& focusPoint)
	{
		m_EditorCamera.focalPoint = focusPoint;
		m_CameraMode = CameraMode::FLYCAM;
		if (m_EditorCamera.distance > m_MinFocusDistance)
		{
			m_EditorCamera.distance -= m_EditorCamera.distance - m_MinFocusDistance;
			m_EditorCamera.position = m_EditorCamera.focalPoint - GetForwardDirection() * m_EditorCamera.distance;
		}
		m_EditorCamera.position = m_EditorCamera.focalPoint - GetForwardDirection() * m_EditorCamera.distance;
		UpdateCameraView();
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
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 50.0f);	// max speed = 50

		return speed;
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.3f;
	}


	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		if (Input::IsMouseButtonDown(MouseButton::Right))
		{
			m_NormalSpeed += e.GetYOffset() * 0.3f * m_NormalSpeed;
			m_NormalSpeed = std::clamp(m_NormalSpeed, MIN_SPEED, MAX_SPEED);
		}
		else
		{
			MouseZoom(e.GetYOffset() * 0.1f);
			UpdateCameraView();
		}

		return true;
	}


	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_EditorCamera.focalPoint -= GetRightDirection() * delta.x * xSpeed * m_EditorCamera.distance;
		m_EditorCamera.focalPoint += GetUpDirection() * delta.y * ySpeed * m_EditorCamera.distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		m_EditorCamera.yawDelta += yawSign * delta.x * RotationSpeed();
		m_EditorCamera.pitchDelta += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_EditorCamera.distance -= delta * ZoomSpeed();
		const glm::vec3 forwardDir = GetForwardDirection();
		m_EditorCamera.position = m_EditorCamera.focalPoint - forwardDir * m_EditorCamera.distance;
		if (m_EditorCamera.distance < 1.0f)
		{
			m_EditorCamera.focalPoint += forwardDir * m_EditorCamera.distance;
			m_EditorCamera.distance = 1.0f;
		}
		m_EditorCamera.positionDelta -= delta * ZoomSpeed() * forwardDir;
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
		return m_EditorCamera.focalPoint - GetForwardDirection() * m_EditorCamera.distance +  m_EditorCamera.positionDelta;
	}


	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_EditorCamera.pitch - m_EditorCamera.pitchDelta, -m_EditorCamera.yaw - m_EditorCamera.yawDelta, 0.0f));
	}


	static void DisableMouse()
	{
		Input::SetCursorMode(CursorMode::Locked);
		// UI::SetInputEnabled(false);
	}

	static void EnableMouse()
	{
		Input::SetCursorMode(CursorMode::Normal);
		// UI::SetInputEnabled(true);
	}

	void EditorCamera::OnUpdate(Unique<Timestep>& ts) 
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.002f;

		// if (!m_IsActive)
		// {
			// if (!UI::IsInputEnabled())
			// 	UI::SetInputEnabled(true);

		// 	return;
		// }

		if (Input::IsMouseButtonDown(MouseButton::Right) && !Input::IsKeyDown(KeyCode::LeftAlt))
		{
			m_CameraMode = CameraMode::FLYCAM;
			DisableMouse();
			const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

			const float speed = GetCameraSpeed();

			if (Input::IsKeyDown(KeyCode::Q))
				m_EditorCamera.positionDelta -= ts->GetSeconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
			if (Input::IsKeyDown(KeyCode::E))
				m_EditorCamera.positionDelta += ts->GetSeconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
			if (Input::IsKeyDown(KeyCode::S))
				m_EditorCamera.positionDelta -= ts->GetSeconds() * speed * m_EditorCamera.direction;
			if (Input::IsKeyDown(KeyCode::W))
				m_EditorCamera.positionDelta += ts->GetSeconds() * speed * m_EditorCamera.direction;
			if (Input::IsKeyDown(KeyCode::A))
				m_EditorCamera.positionDelta -= ts->GetSeconds() * speed * m_EditorCamera.rightDirection;
			if (Input::IsKeyDown(KeyCode::D))
				m_EditorCamera.positionDelta += ts->GetSeconds() * speed * m_EditorCamera.rightDirection;

			constexpr float maxRate{ 0.12f };
			m_EditorCamera.yawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
			m_EditorCamera.pitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

			m_EditorCamera.rightDirection = glm::cross(m_EditorCamera.direction, glm::vec3{ 0.f, yawSign, 0.f });

			m_EditorCamera.direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_EditorCamera.pitchDelta, m_EditorCamera.rightDirection),
				glm::angleAxis(-m_EditorCamera.yawDelta, glm::vec3{ 0.f, yawSign, 0.f }))), m_EditorCamera.direction);

			const float distance = glm::distance(m_EditorCamera.focalPoint, m_EditorCamera.position);
			m_EditorCamera.focalPoint = m_EditorCamera.position + GetForwardDirection() * distance;
			m_EditorCamera.distance = distance;
		}
		else if (Input::IsKeyDown(KeyCode::LeftAlt))
		{
			m_CameraMode = CameraMode::ARCBALL;

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
				MouseZoom((delta.x + delta.y) * 0.1f);
			}
			else
				EnableMouse();
		}
		else
		{
			EnableMouse();
		}

		m_InitialMousePosition = mouse;
		m_EditorCamera.position += m_EditorCamera.positionDelta;
		m_EditorCamera.yaw += m_EditorCamera.yawDelta;
		m_EditorCamera.pitch += m_EditorCamera.pitchDelta;

		if (m_CameraMode == CameraMode::ARCBALL)
			m_EditorCamera.position = CalculatePosition();

		UpdateCameraView();
	}

	
	void EditorCamera::OnEvent(Event& event) 
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScroll(e); });
	}
}
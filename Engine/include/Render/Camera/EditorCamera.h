#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "Core/Input.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"


namespace Cober {

	class CB_API EditorCamera : public Camera
	{

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool persp = true);
		virtual ~EditorCamera();

		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;

		void SetViewportSize(float width, float height) override;

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		float GetCameraSpeed() const;

	private:
		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& events);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		bool m_IsActive = true;
		float m_NormalSpeed{ 0.0005f };

		uint32_t m_ViewportWidth{ 1280 }, m_ViewportHeight{ 720 };

		float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
		friend class Editor;
	};
}

#endif
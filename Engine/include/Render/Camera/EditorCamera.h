#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "Core/Input.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"


namespace Cober {

	class EditorCamera : public Camera
	{

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho = false);
		virtual ~EditorCamera();

		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;

		void SetViewportSize(float width, float height);

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		CameraSettings& GetSettings() { return m_EditorCamera; }
		bool& IsPerspective() { return m_EditorCamera.perspectiveProjection; }

		// const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		// glm::mat4 GetViewProjection() const { return GetProjectionMatrix() * m_ViewMatrix; }

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetFocalPoint() const { return m_EditorCamera.focalPoint; }
		inline float GetDistance() const { return m_EditorCamera.distance; }

		const glm::vec3& GetPosition() const { return m_EditorCamera.position; }
		inline float GetPitch() const 	{ return m_EditorCamera.pitch; }
		inline float GetYaw() const 	{ return m_EditorCamera.yaw; }
		inline float GetRoll() const 	{ return m_EditorCamera.roll; }
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
		CameraSettings m_EditorCamera;

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		bool m_IsActive = true;
		float m_NormalSpeed{ 0.0005f };

		uint32_t m_ViewportWidth{ 1280 }, m_ViewportHeight{ 720 };

		float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
		friend class Editor;
	};
}

#endif
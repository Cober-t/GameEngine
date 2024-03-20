#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "Core/Input.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"


namespace Cober {

	enum class CameraMode
	{
		NONE, FLYCAM, ARCBALL
	};


	class EditorCamera : public Camera
	{

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho = false);
		virtual ~EditorCamera();

		void Focus(const glm::vec3& focusPoint);
		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;

		void SetViewportSize(float width, float height);

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		CameraSettings GetEditorCamera() { return m_EditorCamera; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return GetProjectionMatrix() * m_ViewMatrix; }
		glm::mat4 GetUnReversedViewProjection() const { return GetUnReversedProjectionMatrix() * GetViewMatrix(); }

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetFocalPoint() const { return m_EditorCamera.focalPoint; }
		inline float GetDistance() const { return m_EditorCamera.distance; }
		inline void SetDistance(float distance) { m_EditorCamera.distance = distance; }

		const glm::vec3& GetPosition() const { return m_EditorCamera.position; }
		inline float GetPitch() const 	{ return m_EditorCamera.pitch; }
		inline float GetYaw() const 	{ return m_EditorCamera.yaw; }
		inline float GetRoll() const 	{ return m_EditorCamera.roll; }
		float GetCameraSpeed() const;

	private:
		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		glm::mat4 m_ViewMatrix;
		CameraSettings m_EditorCamera;

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;
		float m_RotationSpeed = 0.8f;
		bool m_MouseButtonHeld = false;
		bool m_AltKeyPressed = false;

		bool m_IsActive = true;
		float m_NormalSpeed{ 0.002f };

		CameraMode m_CameraMode{ CameraMode::ARCBALL };

		float m_MinFocusDistance{ 100.0f };
		uint32_t m_ViewportWidth{ 1280 }, m_ViewportHeight{ 720 };

		constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
		friend class Editor;
	};
}

#endif
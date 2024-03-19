#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Cober {

	struct EditorOrthoCamera 
	{
		float fov = 45.0f;
		float aspectRatio = 1.778f;
		float nearClip = 0.01f;
		float farClip = 1000.0f;

		glm::vec3 position   = { 0.0f, 0.0f,  0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, -1.0f };

		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;
		float distance = 3.0f;

		EditorOrthoCamera() = default;
		EditorOrthoCamera(float newFov, float ratio, float nearValue, float farValue)
		 : fov(newFov), aspectRatio(ratio), nearClip(nearValue), farClip(farValue) { };

		~EditorOrthoCamera() = default;
	};

	struct EditorPerspCamera
	{
		float fov = 45.0f;
		float aspectRatio = 1.778f;
		float nearClip = 0.01f;
		float farClip = 1000.0f;

		glm::vec3 position	 = {   2.5f, 2.5f,  3.2f };
		glm::vec3 focalPoint = { 0.625f, 0.57f, 0.5f };

		float pitch = 6.8f;
		float yaw = -6.8f;
		float roll = 0.0f;
		float distance = 5.0f;

		EditorPerspCamera() = default;
		EditorPerspCamera(float newFov, float ratio, float nearValue, float farValue)
		 : fov(newFov), aspectRatio(ratio), nearClip(nearValue), farClip(farValue) { };

		~EditorPerspCamera() = default;
	};


	class EditorCamera : public Camera
	{

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip, bool ortho = false);
		virtual ~EditorCamera();

		void OnUpdate(Unique<Timestep>& ts);
		void OnEvent(Event& event);

		void BlockEvents(bool focus) { m_ViewportFocused = focus; }

		void SetViewportSize(float width, float height);
		void UpdateProjection(bool& ortho);

		EditorOrthoCamera GetOrthoCamera() { return m_OrthoCamera; }

		EditorPerspCamera GetPerspCamera() { return m_PerspCamera; }

		glm::quat GetOrientation() const;

		glm::vec3 GetUpDirection() const;

		glm::vec3 GetRightDirection() const;

		glm::vec3 GetForwardDirection() const;

		inline const glm::mat4& GetProjection() const override { return m_Projection; }

		inline const glm::mat4& GetView() const override { return m_ViewMatrix; }

		inline const glm::mat4 GetPV() const override { return m_Projection * m_ViewMatrix; }

		inline float GetDistance() const { return m_OrthoProjection ? m_OrthoCamera.distance : m_PerspCamera.distance; }

		inline void SetDistance(float distance) { m_OrthoProjection ? m_OrthoCamera.distance = distance : m_PerspCamera.distance = distance; }

		const glm::vec3& GetPosition() const { return m_OrthoProjection ? m_OrthoCamera.position : m_PerspCamera.position; }

		inline float GetPitch() const 	{ return m_OrthoProjection ? m_OrthoCamera.pitch : m_PerspCamera.pitch; }

		inline float GetYaw() const 	{ return m_OrthoProjection ? m_OrthoCamera.yaw : m_PerspCamera.yaw; }

		inline float GetRoll() const 	{ return m_OrthoProjection ? m_OrthoCamera.roll : m_PerspCamera.roll; }

	private:
		void UpdateView();

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		EditorOrthoCamera m_OrthoCamera;
		EditorPerspCamera m_PerspCamera;
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		glm::vec2 m_InitialMousePos = { 0.0f, 0.0f };
		float m_RotationSpeed = 0.8f;
		bool m_ViewportFocused = false;
		bool m_MouseButtonHeld = false;
		bool m_AltKeyPressed = false;
		bool m_OrthoProjection = false;
	};
}

#endif
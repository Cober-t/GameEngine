#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "Camera.h"


namespace Cober {


	class GameCamera : public Camera
	{

	public:
		GameCamera() = default;
		GameCamera(float fov, float width, float height, float nearClip, float farClip, bool ortho = false);
		virtual ~GameCamera();

		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;

		void SetViewportSize(float width, float height);

		CameraSettings GetGameCameraSettings() { return m_GameCamera; }

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		glm::mat4 GetViewProjection() const { return GetProjectionMatrix() * GetViewMatrix(); }

	private:
		void UpdateCameraView();

		glm::vec3 CalculatePosition() const;

	private:
		CameraSettings m_GameCamera;
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}

#endif
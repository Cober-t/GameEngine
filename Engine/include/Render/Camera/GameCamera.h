#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "Camera.h"

namespace Cober {


	class CB_API GameCamera : public Camera
	{

	public:
		GameCamera() = default;
		GameCamera(float fov, float width, float height, float nearClip, float farClip, bool persp = true);
		virtual ~GameCamera();

		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;

		void SetViewportSize(float width, float height) override;

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		glm::mat4 GetViewProjection() const { return GetProjectionMatrix() * GetViewMatrix(); }

	private:
		void UpdateCameraView();

		glm::vec3 CalculatePosition() const;

	private:
		glm::mat4 m_ViewMatrix;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Cober {

	struct CameraSettings
	{
		float aspectRatio = 1.7f;
		float fov = 45.0f;
		float nearClip = 0.01f;
		float farClip = 1000.0f;
		bool perspectiveProjection = true;

		glm::vec3 position	 = {   -5.0f, 5.0f, 5.0f };
		glm::vec3 direction	 = {   0.0f, 0.0f,  0.0f };
		glm::vec3 focalPoint = { 0.625f, 0.57f, 0.5f };

		float pitch, yaw, roll, distance;
		float width, height;

		CameraSettings() = default;
		CameraSettings(float newFov, float widthValue, float heightValue, float nearValue, float farValue, bool persp)
		 : fov(newFov), width(widthValue), height(heightValue), nearClip(nearValue), farClip(farValue), perspectiveProjection(persp) { };

		~CameraSettings() = default;
	};


	class CB_API Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection);
		Camera(float fov, float width, float height, float nearClip, float farClip);
		virtual ~Camera() = default;

		virtual void OnUpdate(Unique<Timestep>& ts) = 0;
		virtual void OnEvent(Event& event) = 0;
		virtual void SetViewportSize(float width, float height) = 0;

		bool IsMainCamera() { return m_IsMainCamera; }
		void SetMainCamera(bool main = true) { m_IsMainCamera = main ; }
		bool& IsPerspective() { return m_CameraSettings.perspectiveProjection; }
		virtual void SetPerspective(bool persp = true) = 0;

		CameraSettings& GetSettings() { return m_CameraSettings; }
		CameraSettings GetSettings() const { return m_CameraSettings; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		void SetViewMatrix(glm::mat4& viewMatrix) { m_ViewMatrix = viewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		void SetProjectionMatrix(const glm::mat4 projection, const glm::mat4 unReversedProjection)
		{
			m_ProjectionMatrix = projection;
		}

		void SetPerspectiveProjectionMatrix(float radFov, float width, float height, float nearClip, float farClip)
		{
			m_ProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearClip, farClip);
		}

		void SetOrthoProjectionMatrix(float leftRight, float topBottom, float nearClip, float farClip)
		{
			m_ProjectionMatrix = glm::ortho(-leftRight, leftRight, -topBottom, topBottom, nearClip, farClip);
		}

		void SetOrthoProjectionMatrix(float left, float right, float bottom, float top, float nearClip, float farClip)
		{
			m_ProjectionMatrix = glm::ortho(-left, right, -bottom, top, nearClip, farClip);
		}

	private:
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		// This must be true to SetViewportSize on Camera Init
		bool m_IsMainCamera = true;

		CameraSettings m_CameraSettings;
	};
}

#endif
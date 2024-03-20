#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Cober {

	struct CameraSettings
	{
		float fov = 45.0f;
		float verticalFov = 0.0f;
		float aspectRatio = 1.778f;
		float nearClip = 0.01f;
		float farClip = 1000.0f;
		bool orthoPerspective = true;

		glm::vec3 position	 = {   -5.0f, 5.0f, 5.0f };
		glm::vec3 direction	 = {   0.0f, 0.0f,  0.0f };
		glm::vec3 focalPoint = { 0.625f, 0.57f, 0.5f };

		float pitch, yaw, roll, distance;
		float width, height;

		float pitchDelta{}, yawDelta{};
		glm::vec3 positionDelta{};
		glm::vec3 rightDirection{};

		CameraSettings() = default;
		CameraSettings(float newFov, float widthValue, float heightValue, float nearValue, float farValue, bool ortho)
		 : fov(newFov), width(widthValue), height(heightValue), nearClip(nearValue), farClip(farValue), orthoPerspective(ortho) { };

		~CameraSettings() = default;
	};


	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection);
		Camera(float fov, float width, float height, float nearClip, float farClip);
		virtual ~Camera() = default;

		virtual void OnUpdate(Unique<Timestep>& ts) = 0;
		virtual void OnEvent(Event& event) = 0;

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		void SetViewMatrix(glm::mat4& viewMatrix) { m_ViewMatrix = viewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }

		void SetProjectionMatrix(const glm::mat4 projection, const glm::mat4 unReversedProjection)
		{
			m_ProjectionMatrix = projection;
			m_UnReversedProjectionMatrix = unReversedProjection;
		}

		void SetPerspectiveProjectionMatrix(float radFov, float width, float height, float nearClip, float farClip)
		{
			m_ProjectionMatrix = glm::perspectiveFov(radFov, width, height, farClip, nearClip);
			m_UnReversedProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearClip, farClip);
		}

		void SetOrthoProjectionMatrix(float width, float height, float nearClip, float farClip)
		{
			//TODO(Karim): Make sure this is correct.
			m_ProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farClip, nearClip);
			m_UnReversedProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearClip, farClip);
		}

		float GetExposure() const { return m_Exposure; }
		float& GetExposure() { return m_Exposure; }

	protected:
		float m_Exposure = 0.8f;
		
	private:
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		//Needed in the future for shadow maps and ImGuizmo
		glm::mat4 m_UnReversedProjectionMatrix = glm::mat4(1.0f);
	};
}

#endif
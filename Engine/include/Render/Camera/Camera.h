#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>

namespace Cober {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const { return m_Projection; }
		virtual const glm::mat4& GetView() const { return m_ViewMatrix; }
		virtual const glm::mat4 GetPV() const { return m_Projection * m_ViewMatrix; }
	protected:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}

#endif
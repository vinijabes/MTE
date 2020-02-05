#include "mtepch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Kinematics {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearZ, float farZ)
		:Camera(glm::perspective(glm::radians(fov), aspect, nearZ, farZ))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect, float nearZ, float farZ)
	{
		m_ProjectionMatrix = glm::perspective(fov, aspect, nearZ, farZ);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	glm::vec2 PerspectiveCamera::PixelToWindowSize(int x, int y) const
	{
		return glm::vec2();
	}

}
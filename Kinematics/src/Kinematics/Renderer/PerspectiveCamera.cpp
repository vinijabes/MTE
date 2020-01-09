#include "mtepch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Kinematics {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearZ, float farZ)
		:Camera(glm::perspective(fov, aspect, nearZ, farZ))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect, float nearZ, float farZ)
	{
		m_ProjectionMatrix = glm::perspective(fov, aspect, nearZ, farZ);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}	
}
#pragma once

#include "Camera.h"

namespace Kinematics {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float nearZ, float farZ);

		const glm::vec3& GetPosition() const { return m_Position; }		
		void SetProjection(float left, float right, float bottom, float top);
		virtual glm::vec2 ToWindowPosition(int x, int y) const override { return glm::vec2(); }
		virtual glm::vec2 PixelToWindowSize(int x, int y) const override;

	};
}
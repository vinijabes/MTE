#pragma once

#include "Camera.h"

namespace Kinematics {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		float GetRotation() const { return m_EulerAngles.z; }
		void SetRotation(float rotation) { Camera::SetRotation(glm::vec3(0, 0, rotation)); }

		void SetProjection(float left, float right, float bottom, float top);	

		virtual glm::vec2 ToWindowPosition(int x, int y) const override;
		virtual glm::vec2 PixelToWindowSize(int x, int y) const override;

	private:
		float m_Left, m_Right, m_Top, m_Bottom;
	};
}
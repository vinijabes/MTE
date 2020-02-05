#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Kinematics
{
	class Camera
	{
	public:
		Camera(const glm::mat4& m_ProjectionMatrix) : m_ProjectionMatrix(m_ProjectionMatrix), m_ViewMatrix(1.0f), m_Rotation(glm::vec3(0.0f, 0.0f, 0.0f))
		{

		}

		void RecalculateViewMatrix();

		glm::vec3 GetRotationEulerAngles() { return m_EulerAngles; }
		glm::quat GetQuaternion() { return m_Rotation; }

		void SetRotation(glm::vec3 rotation) 
		{
			m_Rotation = glm::quat(rotation);
			RecalculateViewMatrix();
		}
		void SetPosition(const glm::vec3& position) 
		{ 
			m_Position = position; 
			RecalculateViewMatrix(); 
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec2 ToWindowPosition(glm::vec2 pos) const { return ToWindowPosition(pos.x, pos.y); }
		virtual glm::vec2 ToWindowPosition(int x, int y) const = 0;

		glm::vec2 PixelToWindowSize(glm::vec2 pos) const { return PixelToWindowSize(pos.x, pos.y); }
		virtual glm::vec2 PixelToWindowSize(int x, int y) const = 0;
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::quat m_Rotation;
		glm::vec3 m_EulerAngles;
	};
}
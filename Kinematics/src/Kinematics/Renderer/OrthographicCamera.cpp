#include "mtepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "RenderCommand.h"

namespace Kinematics {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec2 OrthographicCamera::ToWindowPosition(int x, int y) const
	{
		auto window = RenderCommand::GetWindow();

		glm::vec2 pos(0);
		pos.x = m_Left + ((float)((float)x + (float)window->GetWidth()/2)/ (float)window->GetWidth() - 0.5f)* std::abs(m_Left - m_Right);
		pos.y = m_Top - ((float)((float)y + (float)window->GetHeight()/2)/ (float)window->GetHeight() - 0.5f) * std::abs(m_Bottom - m_Top);

		return pos;
	}

	glm::vec2 OrthographicCamera::PixelToWindowSize(int x, int y) const
	{
		auto window = RenderCommand::GetWindow();
		glm::vec2 size((double)x / window->GetWidth() * std::abs(m_Left - m_Right), (double)y / window->GetHeight() * std::abs(m_Bottom - m_Top));

		return size;
	}
}
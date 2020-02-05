#include "mtepch.h"
#include "Window.h"
namespace Kinematics {
	namespace UI {
		Window::Window()
		{
			m_Body = CreateRef<Panel>();
			auto layout = CreateRef<BoxLayout>();

			layout->SetOrientation(Orientation::Vertical);
			layout->SetAlignment(Alignment::Fill);

			m_Body->SetLayout(layout);
			UIElementInterface::PushChild(m_Body);
		}

		void Window::Draw(Camera& camera, glm::vec2 parentPos)
		{
			m_Body->Draw(camera, m_Position);
		}
	}
}
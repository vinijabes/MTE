#include "mtepch.h"
#include "TextBox.h"


namespace Kinematics {
	namespace UI {

		TextBox::TextBox()
		{
		}

		void TextBox::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto pos = m_Position;
			auto size = m_Size;
			auto drawingPos = pos + size / 2.f + parentPos;
			
			if(m_Text)
				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size.x, -size.y), m_Text->GetTexture());

			DrawChildren(camera, pos + parentPos);
		}

		void TextBox::Update(Timestep ts)
		{
			if (m_Text)
			{
				m_Size = glm::vec2(m_Text->GetTexture()->GetWidth(), m_Text->GetTexture()->GetHeight());
			}
		}
	}
}
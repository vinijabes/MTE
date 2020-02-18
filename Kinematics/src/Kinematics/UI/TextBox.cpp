#include "mtepch.h"
#include "TextBox.h"


namespace Kinematics {
	namespace UI {

		TextBox::TextBox()
		{
			m_Text = CreateRef<Text>(FontFace::DEFAULT);
		}

		void TextBox::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto pos = m_Position;
			auto size = GetSize();
			auto drawingPos = pos + size / 2.f + parentPos;
			
			if(m_Text && m_Text->GetTexture())
				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size.x, -size.y), m_Text->GetTexture());

			DrawChildren(camera, pos + parentPos);
		}

		void TextBox::Update(Timestep ts)
		{
			RecalculateSize();
		}
		glm::vec2 TextBox::GetPreferredSize() const
		{
			if (m_Text)
			{
				return glm::vec2(
					std::max(m_Size.x, (float)m_Text->GetPixelWidth()),
					std::max(m_Size.y, (float)m_Text->GetPixelHeight())
				);
			}

			return UIElementInterface::GetPreferredSize();
		}
		
		void TextBox::RecalculateSize()
		{
			if (m_Text)
			{
				m_Size = glm::vec2(m_Text->GetPixelWidth(), m_Text->GetPixelHeight());
			}
		}
	}
}
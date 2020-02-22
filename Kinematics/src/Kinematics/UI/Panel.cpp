#include "mtepch.h"
#include "Panel.h"


namespace Kinematics {
	namespace UI {
		Panel::Panel()
			: m_Color(0.f), m_HighlightColor(0.f), m_CurrentColor(0.f)
		{
			m_OnEnter = m_OnEnter + CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
				if(m_HighlightColor != glm::vec4(0.f))
					m_CurrentColor = m_HighlightColor;
			});

			m_OnOut = m_OnOut + CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
				m_CurrentColor = m_Color;
			});
		}

		void Panel::Draw(Camera& camera, glm::vec2 parentPos)
		{
			if (m_CurrentColor != glm::vec4(0.f))
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_CurrentColor);
			} 
			else if (m_Theme)
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Theme->m_Color);
			}

			DrawChildren(camera, parentPos + m_Position);
		}
	}
}
#pragma once

#include "UIElementInterface.h"

namespace Kinematics
{
	namespace UI {
		class Container : public UIElementInterface
		{
		public:
			Container()
				: m_Color(0), m_HighlighColor(0), m_ClickColor(0)
			{
				m_OnEnter = m_OnEnter + CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
					m_CurrentColor = m_HighlighColor;
					});

				m_OnOut = m_OnOut + CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
					m_CurrentColor = m_Color;
					});

				m_OnClick = m_OnClick + CreateRef<FunctionCallback<void, MouseButtonReleasedEvent&>>([this](MouseButtonReleasedEvent& e) {
					m_CurrentColor = m_ClickColor;
					});
			}

			virtual void Draw(Camera& camera, glm::vec2 parentPos = glm::vec2(0)) override
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_CurrentColor);
				DrawChildren(camera, pos + parentPos);
			}

			void SetColor(glm::vec4 color)
			{
				if (color != m_Color)
				{
					m_Color = color;
					m_CurrentColor = color;
				}
			}

			void SetHighlightColor(glm::vec4 color)
			{
				if (color != m_HighlighColor)
				{
					m_HighlighColor = color;
				}
			}

			void SetClickColor(glm::vec4 color)
			{
				if (color != m_ClickColor)
				{
					m_ClickColor = color;
				}
			}

		private:
			glm::vec4 m_CurrentColor;

			glm::vec4 m_Color;
			glm::vec4 m_HighlighColor;
			glm::vec4 m_ClickColor;
		};
	}
}
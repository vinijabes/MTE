#pragma once

#include "UIElementInterface.h"

namespace Kinematics
{
	namespace UI
	{
		class Panel : public UIElementInterface
		{
		public:
			Panel();
			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;

			virtual void SetColor(const glm::vec4& color) 
			{ 
				m_Color = color; 
				m_CurrentColor = color;
			}
			void SetHighlightColor(glm::vec4 color) { m_HighlightColor = color; }
		protected:
			glm::vec4 m_Color;

			glm::vec4 m_HighlightColor;
			glm::vec4 m_CurrentColor;
		};
	}
}
#pragma once

#include "UIElementInterface.h"

namespace Kinematics
{
	namespace UI
	{
		class Panel : public UIElementInterface
		{
		public:
			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;

			void SetColor(glm::vec4 color) { m_Color = color; }
			void SetHighlightColor(glm::vec4 color) { m_HighlightColor = color; }
		private:
			glm::vec4 m_Color;

			glm::vec4 m_HighlightColor;
		};
	}
}
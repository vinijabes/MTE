#include "mtepch.h"
#include "Theme.h"

namespace Kinematics
{
	Ref<Theme> Theme::DARK_THEME = CreateRef<DarkTheme>();

	DarkTheme::DarkTheme()
	{
		m_Margin = 8;
		m_Padding = 8;

		m_FontHeaderSize = 28;
		m_FontTitleSize = 38;
		m_FontSize = 20;

		m_Color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
		m_ShadowColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.5f);

		m_TextColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		m_DisabledTextColor = glm::vec4(0.8f, 0.8f, 0.8f, 0.8f);;

		m_WindowTitleFocusedColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_WindowTitleUnfocusedColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);

		m_WindowHeaderColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	}
}

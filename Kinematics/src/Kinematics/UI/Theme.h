#pragma once

#include "Kinematics/Renderer/FontFace.h"

namespace Kinematics
{
	class Theme
	{
	public:
		uint32_t m_Margin;
		uint32_t m_Padding;

		uint32_t m_FontHeaderSize;
		uint32_t m_FontTitleSize;
		uint32_t m_FontSize;

		glm::vec4 m_Color;
		glm::vec4 m_ShadowColor;

		glm::vec4 m_TextColor;
		glm::vec4 m_DisabledTextColor;

		glm::vec4 m_WindowTitleFocusedColor;
		glm::vec4 m_WindowTitleUnfocusedColor;

		glm::vec4 m_WindowHeaderColor;


		static Ref<Theme> DARK_THEME;
	};

	class DarkTheme : public Theme
	{
	public:
		DarkTheme();
	};
}
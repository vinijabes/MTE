#pragma once

#include "Texture.h"
#include "Kinematics/Core/Core.h"

#include "FontFace.h"

namespace Kinematics {
	class Text
	{
	public:
		Text(Ref<FontFace> font);

		void SetText(const std::string& text);
		void SetFontSize(uint32_t size);
		void SetColor(glm::vec4 color);

		Ref<Texture2D> GetTexture() { return m_TextTexture; }
		std::string GetRawText() const { return m_Text; }

		void RecreateTextTexture();

	private:
		Ref<FontFace> m_Font;
		Ref<Texture2D> m_TextTexture;
		std::string m_Text;

		uint32_t m_FontSize;

		glm::vec4 m_Color;
	};
}
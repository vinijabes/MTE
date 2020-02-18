#pragma once

#include "Texture.h"
#include "Kinematics/Core/Core.h"

#include "FontFace.h"


#include <list>

namespace Kinematics {
	class Text
	{
	public:
		Text(Ref<FontFace> font);

		virtual void SetText(const std::string& text);
		void SetFontSize(uint32_t size);
		void SetColor(glm::vec4 color);

		Ref<Texture2D> GetTexture() { return m_TextTexture; }
		std::string GetRawText() const { return m_Text; }

		uint32_t GetPixelWidth() const
		{
			if (m_TextTexture) return m_TextTexture->GetWidth();
			return 0;
		}

		uint32_t GetPixelHeight() const
		{
			return m_Font->GetAscender() + m_Font->GetDescender();
		}

		virtual void RecreateTextTexture();

	protected:
		Ref<FontFace> m_Font;
		Ref<Texture2D> m_TextTexture;
		std::string m_Text;

		uint32_t m_FontSize;

		glm::vec4 m_Color;
	};

	class CharacterList : public Text
	{
	public:
		CharacterList(Ref<FontFace> font = FontFace::DEFAULT);

		void PushCharacterList(const Ref<CharacterList>& list, size_t pos = -1);
		void PushCharacterList(const CharacterList& list, size_t pos = -1);
		virtual void SetText(const std::string& text) override;

		void PushCharacter(int character, size_t pos = -1);
		void RemoveCharacter(size_t pos);
		void RemoveCharacter(size_t init, size_t end);

		void Clear();

		virtual void RecreateTextTexture() override;

		std::string GetString(size_t init, size_t end);

		size_t GetSize() const { return m_Characters.size(); }
		std::list<Character> const GetCharacters() const { return m_Characters; }
		glm::ivec4 GetCharacterRect(size_t pos) const;

	protected:
		std::list<Character> m_Characters;
	};
}
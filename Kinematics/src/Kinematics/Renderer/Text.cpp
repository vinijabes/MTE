#include "mtepch.h"
#include "Text.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Buffer.h"

#include "OrthographicCamera.h"

namespace Kinematics
{
	Text::Text(Ref<FontFace> font)
		: m_Font(font), m_FontSize(48), m_Color(1.f)
	{
		m_Font->Load(m_FontSize);
	}

	void Text::SetText(const std::string& text)
	{
		if (m_Text != text)
		{
			m_Text = text;
			Text::RecreateTextTexture();
		}
	}

	void Text::SetFontSize(uint32_t size)
	{
		if (m_FontSize != size)
		{
			m_FontSize = size;
			m_Font->Load(m_FontSize);
			RecreateTextTexture();
		}
	}

	void Text::SetColor(glm::vec4 color)
	{
		if (m_Color != color)
		{
			m_Color = color;
			RecreateTextTexture();
		}
	}

	void Text::RecreateTextTexture()
	{
		std::string::const_iterator c;

		uint32_t x = 0;

		for (c = m_Text.begin(); c != m_Text.end(); c++)
		{
			Character ch = m_Font->Get(*c);
			x += (ch.Advance >> 6);
		}

		m_TextTexture = Texture2D::Create(x, m_Font->GetAscender() + m_Font->GetDescender(), WrappingOption::KINEMATICS_CLAMP_TO_EDGE, WrappingOption::KINEMATICS_CLAMP_TO_EDGE);
		auto frameBuffer = FrameBuffer::Create();
		frameBuffer->SetTargetTexture(m_TextTexture);

		RenderCommand::SetViewport(0, 0, x, m_Font->GetAscender() + m_Font->GetDescender());
		Renderer2D::BeginScene(OrthographicCamera(0, x, 0, m_Font->GetAscender() + m_Font->GetDescender()));

		uint32_t xIndex = 0;
		Kinematics::RenderCommand::EnableAlphaBlending();
		for (c = m_Text.begin(); c != m_Text.end(); c++)
		{
			Character ch = m_Font->Get(*c);

			float xpos = xIndex + (float)ch.Bearing.x;

			float w = ch.Size.x;
			float h = ch.Size.y;

			Renderer2D::RenderChar(glm::vec2(xpos + w / 2, m_Font->GetAscender() - ch.Bearing.y + h / 2.f), glm::vec2(w, h), ch, m_Color);
			xIndex += (ch.Advance >> 6);
		}

		Kinematics::RenderCommand::DisableAlphaBlending();
		Renderer2D::EndScene();

		RenderCommand::SetViewport(0, 0, RenderCommand::GetWindow()->GetWidth(), RenderCommand::GetWindow()->GetHeight());
	}

	CharacterList::CharacterList(Ref<FontFace> font)
		: Text(font)
	{
	}

	void CharacterList::PushCharacterList(const Ref<CharacterList>& list, size_t pos)
	{
		PushCharacterList(*list, pos);
	}

	void CharacterList::PushCharacterList(const CharacterList& list, size_t pos)
	{
		std::list<Character>::iterator it;
		if (pos != -1)
		{
			it = m_Characters.begin();
			std::advance(it, pos);
		}
		else
		{
			it = m_Characters.end();
		}

		m_Characters.insert(it, list.m_Characters.begin(), list.m_Characters.end());
		RecreateTextTexture();
	}

	void CharacterList::SetText(const std::string& text)
	{
		Clear();
		for (auto c : text)
		{
			m_Characters.push_back(m_Font->Get(c));
		}
		RecreateTextTexture();
	}

	void CharacterList::PushCharacter(int character, size_t pos)
	{
		std::list<Character>::iterator it;
		if (pos != -1)
		{
			it = m_Characters.begin();
			std::advance(it, pos);
		}
		else
		{
			it = m_Characters.end();
		}

		m_Characters.insert(it, m_Font->Get(character));
		RecreateTextTexture();
	}

	void CharacterList::RemoveCharacter(size_t pos)
	{
		RemoveCharacter(pos, pos + 1);
	}

	void CharacterList::RemoveCharacter(size_t init, size_t end)
	{
		if (init < 0) init = 0;
		if (end > m_Characters.size()) end = m_Characters.size();

		auto itInit = m_Characters.begin();
		std::advance(itInit, init);

		auto itEnd = m_Characters.begin();
		std::advance(itEnd, end);

		m_Characters.erase(itInit, itEnd);
		RecreateTextTexture();
	}

	void CharacterList::Clear()
	{
		m_Characters.clear();
		RecreateTextTexture();
	}

	void CharacterList::RecreateTextTexture()
	{
		std::list<Character>::const_iterator c;

		uint32_t x = 0;

		for (c = m_Characters.begin(); c != m_Characters.end(); c++)
		{
			x += (c->Advance >> 6);
		}

		m_TextTexture = Texture2D::Create(x, m_Font->GetAscender() + m_Font->GetDescender(), WrappingOption::KINEMATICS_CLAMP_TO_EDGE, WrappingOption::KINEMATICS_CLAMP_TO_EDGE);
		auto frameBuffer = FrameBuffer::Create();
		frameBuffer->SetTargetTexture(m_TextTexture);

		RenderCommand::SetViewport(0, 0, x, m_Font->GetAscender() + m_Font->GetDescender());
		Renderer2D::BeginScene(OrthographicCamera(0, x, 0, m_Font->GetAscender() + m_Font->GetDescender()));
		
		uint32_t xIndex = 0;
		Kinematics::RenderCommand::EnableAlphaBlending();
		for (c = m_Characters.begin(); c != m_Characters.end(); c++)
		{
			Character ch = *c;

			float xpos = xIndex + (float)ch.Bearing.x;

			float w = ch.Size.x;
			float h = ch.Size.y;

			Renderer2D::RenderChar(glm::vec2(xpos + w / 2, m_Font->GetAscender() - ch.Bearing.y + h/2.f), glm::vec2(w, h), ch, m_Color);
			xIndex += (ch.Advance >> 6);
		}

		Kinematics::RenderCommand::DisableAlphaBlending();
		Renderer2D::EndScene();

		RenderCommand::SetViewport(0, 0, RenderCommand::GetWindow()->GetWidth(), RenderCommand::GetWindow()->GetHeight());
	}

	std::string CharacterList::GetString(size_t init, size_t end)
	{
		if (init < 0) init = 0;
		if (end > m_Characters.size()) end = m_Characters.size();

		std::string result;
		result.reserve(end - init);

		auto it = m_Characters.cbegin();
		std::advance(it, init);

		for (int i = init; i < end; ++i, ++it)
		{
			result += it->UCode;
		}

		return result;
	}

	glm::ivec4 CharacterList::GetCharacterRect(size_t pos) const
	{
		int x = 0;

		auto it = m_Characters.cbegin();
		for (; pos > 1; ++it, --pos)
		{
			x += (it->Advance >> 6);
		}

		auto y = m_Font->GetMaxVerticalBearing() - it->Bearing.y;
		return glm::ivec4(x, y, x + (it->Advance >> 6), y + it->Size.y);
	}
}
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
			RecreateTextTexture();
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

		uint32_t x = 0, y = 0;

		for (c = m_Text.begin(); c != m_Text.end(); c++)
		{
			Character ch = m_Font->Get(*c);
			x += (ch.Advance >> 6);
			if (ch.Size.y > y) y = ch.Size.y;
		}

		m_TextTexture = Texture2D::Create(x, y, WrappingOption::KINEMATICS_CLAMP_TO_EDGE, WrappingOption::KINEMATICS_CLAMP_TO_EDGE);
		auto frameBuffer = FrameBuffer::Create();
		frameBuffer->SetTargetTexture(m_TextTexture);

		RenderCommand::SetViewport(0, 0, x, y);
		uint32_t xIndex = 0;
		Renderer2D::BeginScene(OrthographicCamera(0, x, 0, y));
		Kinematics::RenderCommand::EnableAlphaBlending();
		for (c = m_Text.begin(); c != m_Text.end(); c++)
		{
			Character ch = m_Font->Get(*c);

			float xpos = xIndex + (float)ch.Bearing.x / 2;
			float ypos = y - (ch.Size.y - ch.Bearing.y);

			float w = ch.Size.x;
			float h = ch.Size.y;

			Renderer2D::RenderChar(glm::vec2(xpos + w/2, ypos - h/2), glm::vec2(w, h), ch, m_Color);
			xIndex += (ch.Advance >> 6);
		}

		Kinematics::RenderCommand::DisableAlphaBlending();
		Renderer2D::EndScene();

		RenderCommand::SetViewport(0, 0, RenderCommand::GetWindow()->GetWidth(), RenderCommand::GetWindow()->GetHeight());
	}
}
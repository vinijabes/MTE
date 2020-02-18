#include "mtepch.h"
#include "TextArea.h"

#include "Kinematics/Core/KeyCodes.h"

namespace Kinematics {
	namespace UI {
		TextLine::TextLine()
			: m_Selection(0)
		{
			auto boxLayout = CreateRef<BoxLayout>();
			boxLayout->SetMargin(0);

			m_String = CreateRef<CharacterList>();

			m_SelectionBox = CreateRef<Panel>();
			m_SelectionBox->SetColor(glm::vec4(0.f, 0.63f, 0.63f, 0.5f));
			m_TextBox = CreateRef<TextBox>();
			m_TextBox->SetText(m_String);
			UIElementInterface::SetLayout(boxLayout);

			PushChild(m_TextBox);
		}

		void TextLine::Draw(Camera& camera, glm::vec2 pos)
		{
			m_SelectionBox->Draw(camera, pos + m_Position);
			DrawChildren(camera, pos + m_Position);
		}

		void TextLine::SetSelection(glm::uvec2 selection)
		{
			m_Selection = selection;
			UpdateSelectionBox();
		}

		void TextLine::SetSelection(uint32_t start, uint32_t end)
		{
			SetSelection(glm::uvec2(start, end));
		}

		glm::vec2 TextLine::GetCharacterPosition(uint32_t pos) const
		{
			return m_String->GetCharacterRect(pos);
		}

		glm::vec2 TextLine::GetCharacterAbsolutePosition(uint32_t pos) const
		{
			return GetCharacterPosition(pos) + GetAbsolutePosition();
		}

		glm::vec4 TextLine::GetCharacterRect(uint32_t pos) const
		{
			return m_String->GetCharacterRect(pos);
		}

		int TextLine::GetCharPosFromXCoords(float pos) const
		{
			if (pos < 0) return 0;

			for (size_t i = 1; i <= GetLength(); i++)
			{
				auto rect = GetCharacterRect(i);
				if (pos >= rect.x&& pos <= rect.z) return i;
			}
			return GetLength() + 1;
		}

		size_t TextLine::GetLength() const
		{
			return m_String->GetSize();
		}

		void TextLine::SetText(const std::string& text)
		{
			m_String->SetText(text);
		}

		void TextLine::SetFontColor(const glm::vec4& color)
		{
			m_String->SetColor(color);
		}

		void TextLine::UpdateSelectionBox()
		{
			if (m_Selection.x - m_Selection.y == 0)
			{
				m_SelectionBox->SetSize({ 0, m_String->GetPixelHeight() });
				return;
			}

			auto width = 0;
			for (int i = m_Selection.x; i < m_Selection.y; i++)
			{
				auto rect = m_String->GetCharacterRect(i);

				width += rect.z - rect.x;
			}

			auto startX = m_String->GetCharacterRect(m_Selection.x).z;
			if (m_Selection.x == 0)
				startX = m_String->GetCharacterRect(m_Selection.x).x;

			auto endX = m_String->GetCharacterRect(m_Selection.y).z;

			m_SelectionBox->SetSize({ endX - startX, m_String->GetPixelHeight() });
			m_SelectionBox->SetPosition({ startX, 0u });
		}

		TextArea::TextArea()
			: m_Cursor(0), m_Selection(0)
		{
			auto boxLayout = CreateRef<BoxLayout>();
			boxLayout->SetMargin(8);
			boxLayout->SetOrientation(Orientation::Vertical);
			boxLayout->SetAlignment(Alignment::Minimum);

			m_ContainerPanel->SetColor(glm::vec4(1.f));
			SetContainerLayout(boxLayout);

			m_CursorPanel = CreateRef<Panel>();
			m_CursorPanel->SetSize({ 2.f, 0.f });
			m_CursorPanel->SetColor({ 0.f, 0.f, 0.f, 1.f });
			SetFixedSize({ 200.f, 200.f });

			PushBackLine();

			m_OnChar = m_OnChar + CreateRef<FunctionCallback<void, CharacterEvent&>>([this](CharacterEvent& e) { HandleCharacterInput(e); });
			m_OnInput = m_OnInput + CreateRef<FunctionCallback<void, KeyPressedEvent&>>([this](KeyPressedEvent& e) {
				HandleCommands(e);
				if (e.GetKeyCode() >= KINEMATICS_KEY_RIGHT && e.GetKeyCode() <= KINEMATICS_KEY_UP)
					HandleDirectionalKeys(e);
				});

			m_OnClick = m_OnClick + CreateRef<FunctionCallback<void, MouseButtonReleasedEvent&>>([this](MouseButtonReleasedEvent& e) {
				HandleMouseClick(e);
			});

		}

		void TextArea::Draw(Camera& camera, glm::vec2 pos)
		{
			ScrollPanel::Draw(camera, pos);
			m_CursorPanel->Draw(camera, pos - m_ScrollPosition + m_Position + m_Lines[m_Cursor.y]->GetPosition());
		}

		void TextArea::Update(Timestep ts)
		{
			UpdateCursorPanelPosition();
			ScrollPanel::Update(ts);
		}

		void TextArea::SetSelection(glm::uvec4 selection)
		{
			m_Selection = selection;
			UpdateLinesSelection();
		}

		void TextArea::SetSelection(uint32_t startLine, uint32_t endLine, uint32_t startPos, uint32_t endPos)
		{
			SetSelection(glm::uvec4(startLine, endLine, startPos, endPos));
		}

		void TextArea::SetSelectionStart(uint32_t startLine, uint32_t startPos)
		{
			SetSelection(glm::uvec4(startLine, m_Selection.y, startPos, m_Selection.w));
		}

		void TextArea::SetSelectionEnd(uint32_t endLine, uint32_t endPos)
		{
			if (m_Selection.z == 0 && endPos == 0) endPos += 1;
			SetSelection(glm::uvec4(m_Selection.x, endLine, m_Selection.z, endPos));
		}

		bool TextArea::IsSelecting()
		{
			return (m_Selection.x | m_Selection.y | m_Selection.z | m_Selection.w) != 0;
		}

		void TextArea::UpdateLinesSelection()
		{
			for (auto l : m_Lines)
				l->SetSelection({ 0, 0 });

			auto startLine = m_Selection.x;
			auto endLine = m_Selection.y;

			auto startPos = m_Selection.z;
			auto endPos = m_Selection.w;

			if (startLine > endLine)
			{
				std::swap(startLine, endLine);
				std::swap(startPos, endPos);
			}

			if (startLine < 0) startLine = 0;
			if (endLine >= m_Lines.size()) endLine = m_Lines.size() - 1;

			auto lineCount = endLine - startLine;
			if (lineCount < 0) return;
			else if (lineCount == 0 && startPos > endPos) std::swap(startPos, endPos);

			m_Lines[startLine]->SetSelection(
				glm::uvec2(
					startPos,
					(lineCount > 0
						? m_Lines[startLine]->GetLength()
						: (endPos > m_Lines[startLine]->GetLength()
							? m_Lines[startLine]->GetLength()
							: endPos)
						)
				)
			);

			for (int i = startLine + 1; i <= (int)endLine - 1; ++i)
			{
				m_Lines[i]->SetSelection(glm::uvec2(0, m_Lines[i]->GetLength()));
			}

			if (lineCount >= 1)
				m_Lines[endLine]->SetSelection(glm::uvec2(0, m_Lines[endLine]->GetLength() < endPos ? m_Lines[endLine]->GetLength() : endPos));
		}

		void TextArea::PushBackLine()
		{
			m_Lines.push_back(CreateRef<TextLine>());
			m_Lines[m_Lines.size() - 1]->SetFontColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
			PushChild(m_Lines[m_Lines.size() - 1]);
		}

		void TextArea::PushLine()
		{
			//m_Lines[m_Cursor.y]->m_String->PushCharacter(13);

			m_Lines.insert(m_Lines.begin() + ++m_Cursor.y, CreateRef<TextLine>());
			(*(m_Lines.begin() + m_Cursor.y))->SetFontColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
			PushChild(*(m_Lines.begin() + m_Cursor.y), m_Cursor.y);

			m_Cursor.x = 0;
		}

		void TextArea::HandleCharacterInput(CharacterEvent& e)
		{
			if (IsFocused())
			{
				m_Lines[m_Cursor.y]->m_String->PushCharacter(e.GetKeyCode(), m_Cursor.x++);
				m_Lines[m_Cursor.y]->m_TextBox->RecalculateSize();
			}
		}

		void TextArea::HandleCommands(KeyPressedEvent& e)
		{
			if (IsFocused())
			{
				switch (e.GetKeyCode())
				{
				case KINEMATICS_KEY_ENTER:
					PushLine();
					break;
				case KINEMATICS_KEY_BACKSPACE:
					if (m_Cursor.x > 0)
						m_Lines[m_Cursor.y]->m_String->RemoveCharacter(--m_Cursor.x);
					break;
				case KINEMATICS_KEY_DELETE:
					if (m_Cursor.x < m_Lines[m_Cursor.y]->GetLength())
						m_Lines[m_Cursor.y]->m_String->RemoveCharacter(m_Cursor.x);
					break;
				}
			}
		}

		void TextArea::HandleDirectionalKeys(KeyPressedEvent& e)
		{
			if (e.GetModifiers() == KINEMATICS_MOD_SHIFT)
			{
				if (!IsSelecting())
				{
					SetSelectionStart(m_Cursor.y, m_Cursor.x);
					SetSelectionEnd(m_Cursor.y, m_Cursor.x);
				}
			}
			else
			{
				StopSelection();
			}

			auto cursorCoods = GetAbsolutePosition() +  m_CursorPanel->GetPosition() + m_Lines[m_Cursor.y]->GetPosition();
			auto cursorSize = m_CursorPanel->GetSize();
			int y = 0, lineLength, line2Length, x;

			switch (e.GetKeyCode())
			{
			case KINEMATICS_KEY_UP:
				lineLength = m_Lines[m_Cursor.y]->GetLength();
				y = m_Cursor.y - 1;

				if (y >= 0)
				{
					line2Length = m_Lines[y]->GetLength();
					x = GetCursorPositionFromAbsoluteCoords(glm::vec2(cursorCoods.x, m_Lines[y]->GetAbsolutePosition().y)).x + 1;
					if (line2Length > lineLength) ++x;

					SetCursorPosition(glm::uvec2(x, y));
				}
				break;
			case KINEMATICS_KEY_DOWN:
				lineLength = m_Lines[m_Cursor.y]->GetLength();
				y = m_Cursor.y + 1;

				if (y < m_Lines.size())
				{
					line2Length = m_Lines[y]->GetLength();
					x = GetCursorPositionFromAbsoluteCoords(glm::vec2(cursorCoods.x, m_Lines[y]->GetAbsolutePosition().y)).x;
					if (line2Length < lineLength) --x;

					SetCursorPosition(glm::uvec2(x, y));
				}
				break;
			case KINEMATICS_KEY_LEFT:
				SetCursorXPosition(m_Cursor.x - 1);
				break;
			case KINEMATICS_KEY_RIGHT:
				SetCursorXPosition(m_Cursor.x + 1);
				break;
			default:
				break;
			}

			if (IsSelecting())
			{
				SetSelectionEnd(m_Cursor.y, m_Cursor.x);
			}

		}

		void TextArea::HandleMouseClick(MouseButtonReleasedEvent& e)
		{
			auto mousePos = RenderCommand::GetWindow()->GetMousePos();
			SetCursorPosition(GetCursorPositionFromAbsoluteCoords(mousePos + m_ScrollPosition));
		}

		void TextArea::SetCursorPosition(glm::uvec2 pos)
		{
			auto line = pos.y;
			if (line >= m_Lines.size()) line = m_Lines.size() - 1;
			else if (line < 0) line = 0;

			auto linePos = pos.x;
			if (linePos == MAXUINT32) linePos = 0;
			else if (m_Lines[line]->GetLength() < linePos) linePos = m_Lines[line]->GetLength();


			m_Cursor = glm::uvec2(linePos, line);
			UpdateCursorPanelPosition();
		}
		void TextArea::SetCursorXPosition(uint32_t x)
		{
			SetCursorPosition(glm::uvec2(x, m_Cursor.y));
		}
		void TextArea::SetCursorYPosition(uint32_t y)
		{
			SetCursorPosition(glm::uvec2(m_Cursor.x, y));
		}

		glm::uvec2 TextArea::GetCursorPositionFromAbsoluteCoords(glm::vec2 coords)
		{
			return GetCursorPositionFromCoords(coords - GetAbsolutePosition());
		}

		glm::uvec2 TextArea::GetCursorPositionFromCoords(glm::vec2 coords)
		{
			size_t line = 0;
			auto height = 0.f;
			for (size_t i = 0; i < m_Lines.size(); ++i)
			{
				if (height + m_Lines[i]->GetSize().y > coords.y)
				{
					line = i;
					break;
				}
				height += m_Lines[i]->GetSize().y;
			}
			uint32_t xPos = 0;
			if (m_Lines[line]->GetLength())
				xPos = m_Lines[line]->GetCharPosFromXCoords(coords.x - m_Lines[line]->GetPosition().x);
			
			if (xPos > 0) --xPos;

			return glm::uvec2(xPos, line);
		}

		glm::vec2 TextArea::GetCursorCoords()
		{
			return m_CursorPanel->GetPosition();
		}

		void TextArea::UpdateCursorPanelPosition()
		{
			auto line = m_Cursor.y;
			auto linePos = m_Cursor.x;

			auto layout = std::static_pointer_cast<BoxLayout>(m_ContainerPanel->GetLayout());
			auto xPos = (m_Lines[line]->GetLength() > 0 && linePos > 0 ? m_Lines[line]->GetCharacterRect(linePos).z : 0);

			float yPos = 0.f;

			for (size_t i = 0; i < m_Cursor.y; ++i)
			{
				yPos += m_Lines[i]->GetSize().y;
			}

			m_CursorPanel->SetPosition(glm::vec2(xPos, 0));

			auto height = m_Lines[line]->m_String->GetPixelHeight();
			auto width = m_CursorPanel->GetSize().x;
			m_CursorPanel->SetSize(glm::vec2(width, height));

			if (xPos > m_ContainerPanel->GetSize().x + m_ScrollPosition.x - m_Lines[m_Cursor.y]->GetPosition().x)
			{
				m_ContainerPanel->ApplyLayout();
				auto scrollXPos = xPos - m_ContainerPanel->GetSize().x + m_Lines[m_Cursor.y]->GetPosition().x + layout->GetMargin();
				SetScrollPosition(glm::vec2(scrollXPos, m_ScrollPosition.y));
			}
			else if (xPos < m_ScrollPosition.x - m_Lines[m_Cursor.y]->GetPosition().x)
			{
				m_ContainerPanel->ApplyLayout();
				auto scrollXPos = xPos - layout->GetMargin() + m_Lines[m_Cursor.y]->GetPosition().x;
				SetScrollPosition(glm::vec2(scrollXPos, m_ScrollPosition.y));
			}

			if (yPos + m_Lines[m_Cursor.y]->GetSize().y > m_ContainerPanel->GetSize().y + m_ScrollPosition.y)
			{
				auto scrollYPos = yPos + m_Lines[m_Cursor.y]->GetSize().y - m_ContainerPanel->GetSize().y + layout->GetMargin();
				SetScrollPosition(glm::vec2(m_ScrollPosition.x, scrollYPos));
			}
			else if (yPos < m_ScrollPosition.y)
			{
				auto scrollYPos = yPos - layout->GetMargin();
				SetScrollPosition(glm::vec2(m_ScrollPosition.x, scrollYPos));
			}
		}

		void TextArea::StopSelection()
		{
			SetSelection(glm::uvec4(0));
		}
	}
}

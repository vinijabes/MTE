#include "mtepch.h"
#include "TextInput.h"

#include "Kinematics/Core/KeyCodes.h"
#include "Window.h"

namespace Kinematics {
	namespace UI {
		TextInput::TextInput()
			: m_InsertPos(0), m_CursorPos(0), m_SelectionStart(-1), m_SelectionEnd(-1)
		{
			m_Text = CreateRef<CharacterList>(FontFace::DEFAULT);
			m_Text->SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
			m_Text->SetFontSize(36);
			m_TextBox = CreateRef<TextBox>();

			auto layout = CreateRef<BoxLayout>();
			layout->SetMargin(4);
			layout->SetOrientation(Orientation::Horizontal);
			layout->SetAlignment(Alignment::Middle);

			m_CursorPos.x = layout->GetMargin();
			m_CursorPos.y = layout->GetMargin();

			UIElementInterface::SetLayout(layout);

			m_TextBox->SetText(m_Text);

			m_OnInput = m_OnInput + CreateRef < FunctionCallback<void, KeyPressedEvent&>>([this](KeyPressedEvent& e) {
				if (IsFocused())
				{
					HandleCommandKeys(e);
					HandleDirectionalKeys(e);
				}
			});

			m_OnChar = m_OnChar + CreateRef < FunctionCallback<void, CharacterEvent&>>([this](CharacterEvent& e) {
				if (IsFocused())
				{
					HandleCharacterKeys(e);
				}
			});

			m_OnClick = m_OnClick + CreateRef <FunctionCallback<void, MouseButtonReleasedEvent&>>([this](MouseButtonReleasedEvent& e) {
				auto mousePos = RenderCommand::GetWindow()->GetMousePos() - GetAbsolutePosition();
				auto characters = m_Text->GetCharacters();
				auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);

				mousePos.x -= layout->GetMargin();
				int index = 0;
				bool found = false;
				for (auto c : characters)
				{
					index++;
					if (mousePos.x > m_Text->GetCharacterRect(index).z) continue;

					found = true;
					break;
				}

				if (index == m_Text->GetSize() && !found)
				{
					index++;
				}

				SetCursorPos((size_t)index - 1);
				m_MouseDown = false;
			});

			m_OnButtonDown = m_OnButtonDown + CreateRef<FunctionCallback<void, MouseButtonPressedEvent&>>([this](MouseButtonPressedEvent& e) {
				auto mousePos = RenderCommand::GetWindow()->GetMousePos() - GetAbsolutePosition();
				auto characters = m_Text->GetCharacters();
				auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);

				mousePos.x -= layout->GetMargin();
				int index = 0;
				bool found = false;
				for (auto c : characters)
				{
					index++;
					if (mousePos.x > m_Text->GetCharacterRect(index).z) continue;

					found = true;
					break;
				}

				if (index == m_Text->GetSize() && !found)
				{
					index++;
				}

				SetCursorPos((size_t)index - 1);

				if (IsSelecting())
				{
					StopSelection();
				}

				StartSelection();
				m_MouseDown = true;
			});

			m_OnMove = m_OnMove + CreateRef <FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
				if (RenderCommand::GetWindow()->GetButtonState(KINEMATICS_MOUSE_BUTTON_LEFT))
				{
					auto mousePos = RenderCommand::GetWindow()->GetMousePos() - GetAbsolutePosition();
					auto characters = m_Text->GetCharacters();
					auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);

					mousePos.x -= layout->GetMargin();
					int index = 0;
					bool found = false;
					for (auto c : characters)
					{
						index++;
						if (mousePos.x > m_Text->GetCharacterRect(index).z) continue;

						found = true;
						break;
					}

					if (index == m_Text->GetSize() && !found)
					{
						index++;
					}

					if (GetCursorPos() != index - 1)
					{
						SetCursorPos((size_t)index - 1);
						UpdateSelection();
					}
				}
			});

			PushChild(m_TextBox);
		}

		void TextInput::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto pos = m_Position;
			auto size = GetSize();
			auto drawingPos = pos + size / 2.f + parentPos;

			Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), glm::vec4(1.f));

			if (m_DrawSelectionBox)
				DrawSelection(camera, m_Position + parentPos);

			if (m_DrawCursor && IsFocused())
				DrawCursor(camera, m_Position + parentPos);

			DrawChildren(camera, m_Position + parentPos);
		}

		void TextInput::DrawSelection(Camera& camera, glm::vec2 parentPos)
		{
			auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);

			auto startPos = m_SelectionStart;
			auto endPos = m_SelectionEnd;
			if (startPos > endPos)
			{
				auto temp = endPos;
				endPos = startPos;
				startPos = temp;
			}

			if (startPos == endPos) return;

			auto start = m_Text->GetCharacterRect(startPos);
			auto end = m_Text->GetCharacterRect(endPos);


			if (startPos == 0)
				start = glm::vec4(layout->GetMargin());

			auto sizeSelection = glm::vec2(end.z - start.z + 2.f, m_Size.y - 2 * layout->GetMargin());
			auto pos = glm::vec2(start.z + layout->GetMargin(), layout->GetMargin()) + parentPos + sizeSelection / 2.f;

			Renderer2D::DrawQuad(camera.ToWindowPosition(pos), camera.PixelToWindowSize(sizeSelection), glm::vec4(0.f, 0.63f, 0.63f, 0.5f));
		}

		void TextInput::DrawCursor(Camera& camera, glm::vec2 parentPos)
		{
			auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);
			auto cursorSize = glm::vec2(2, m_Size.y);
			auto cursorPos = glm::vec2(m_CursorPos) + parentPos + cursorSize / 2.f;
			cursorPos.x -= 1.f;
			cursorPos.y -= layout->GetMargin();

			Renderer2D::DrawQuad(camera.ToWindowPosition(cursorPos), camera.PixelToWindowSize(cursorSize), glm::vec4(0.f, 0.f, 0.f, 1.f));
		}

		void TextInput::PushCharacter(uint32_t code)
		{
			m_Text->PushCharacter(code, GetCursorPos());
			SetCursorPos(GetCursorPos() + 1);
		}

		void TextInput::PushCharacter(uint32_t code, size_t pos)
		{
			SetCursorPos(pos);
			PushCharacter(code);
		}

		void TextInput::RemoveCharacter()
		{
			m_Text->RemoveCharacter(GetCursorPos() - 1);
			SetCursorPos(GetCursorPos() - 1);
		}

		void TextInput::RemoveCharacter(size_t pos)
		{
			SetCursorPos(pos);
			RemoveCharacter();
		}

		void TextInput::RemoveCharacter(size_t init, size_t end)
		{
			SetCursorPos(end);
			while (end-- != init)
			{
				RemoveCharacter();
			}
		}

		void TextInput::HandleDirectionalKeys(KeyPressedEvent& e)
		{
			if (e.GetKeyCode() == KINEMATICS_KEY_LEFT)
			{
				if (e.GetModifiers() & KINEMATICS_MOD_SHIFT)
				{
					if (!IsSelecting())
						StartSelection();
				}

				SetCursorPos(GetCursorPos() - 1);
			}
			else if (e.GetKeyCode() == KINEMATICS_KEY_RIGHT)
			{
				if (e.GetModifiers() & KINEMATICS_MOD_SHIFT)
				{
					if (!IsSelecting())
						StartSelection();
				}

				SetCursorPos(GetCursorPos() + 1);
			}
			else
			{
				return;
			}

			if (e.GetModifiers() & KINEMATICS_MOD_SHIFT)
			{
				if (IsSelecting())
				{
					UpdateSelection();
				}
			}
			else
			{
				StopSelection();
			}
		}

		void TextInput::HandleCommandKeys(KeyPressedEvent& e)
		{
			if (e.GetKeyCode() == KINEMATICS_KEY_BACKSPACE)
			{
				if (IsSelecting())
				{
					RemoveCharacter(GetSelectionStart(), GetSelectionEnd());
					StopSelection();
				}
				else if (m_InsertPos > 0)
				{
					RemoveCharacter();
				}
			}
			if (e.GetKeyCode() == KINEMATICS_KEY_DELETE)
			{
				if (IsSelecting())
				{
					RemoveCharacter(GetSelectionStart(), GetSelectionEnd());
					StopSelection();
				}
				else if (m_InsertPos < m_Text->GetSize())
				{
					RemoveCharacter(GetCursorPos() + 1);
				}
			}

			if (e.GetModifiers() == KINEMATICS_MOD_CONTROL)
			{
				if (e.GetKeyCode() == KINEMATICS_KEY_A)
				{
					SetCursorPos(0);
					StartSelection();

					SetCursorPos(m_Text->GetSize());
					UpdateSelection();
				}
				else if (e.GetKeyCode() == KINEMATICS_KEY_C)
				{
					if (IsSelecting())
						RenderCommand::GetWindow()->SetClipboardString(m_Text->GetString(GetSelectionStart(), GetSelectionEnd()));
					else
						RenderCommand::GetWindow()->SetClipboardString("");
				}
				else if (e.GetKeyCode() == KINEMATICS_KEY_V)
				{
					std::string input = RenderCommand::GetWindow()->GetClipboardString();
					for (auto c : input)
						PushCharacter(c);
				}
			}
		}

		void TextInput::HandleCharacterKeys(CharacterEvent& e)
		{
			if (IsSelecting())
			{
				RemoveCharacter(GetSelectionStart(), GetSelectionEnd());
				StopSelection();
			}

			PushCharacter(e.GetKeyCode());
		}

		bool TextInput::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
		{
			if (m_MouseDown)
			{
				e.StopPropagation();
			}
			if (!m_MouseDown || IsFocused() && m_MouseDown)
			{
				m_MouseDown = false;
				return UIElementInterface::OnMouseButtonReleasedEvent(e);
			}
			m_MouseDown = false;

			GetWindow()->UpdateFocus(GetPtr());

			return true;
		}

		void TextInput::UpdateSelection()
		{
			if (m_SelectionStart != -1)
			{
				if (m_InsertPos <= m_Text->GetSize())
				{
					m_SelectionEnd = m_InsertPos;
				}

				if (m_SelectionEnd == m_SelectionStart) StopSelection();
			}
		}
	}
}
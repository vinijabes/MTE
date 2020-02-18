#pragma once

#include "Panel.h"
#include "TextBox.h"

namespace Kinematics {
	namespace UI {
		class TextInput : public Panel
		{
		public:
			TextInput();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			void DrawSelection(Camera& camera, glm::vec2 pos);
			void DrawCursor(Camera& camera, glm::vec2 pos);

			void PushCharacter(uint32_t code);
			void PushCharacter(uint32_t code, size_t pos);

			void RemoveCharacter();
			void RemoveCharacter(size_t pos);
			void RemoveCharacter(size_t init, size_t end);

			void inline SetCursorPos(size_t pos) 
			{ 
				if (pos == SIZE_MAX) pos = 0;
				if (pos > m_Text->GetSize()) pos = m_Text->GetSize();

				if (m_InsertPos != pos)
				{
					m_InsertPos = pos; 
					UpdateCursorPos();
				}
			}
			size_t inline GetCursorPos() const { return m_InsertPos; }

			size_t inline GetSelectionStart() const { return std::min(m_SelectionStart, m_SelectionEnd); }
			size_t inline GetSelectionEnd() const { return std::max(m_SelectionStart, m_SelectionEnd); }

			virtual glm::vec2 GetPreferredSize() const override
			{
				glm::vec2 size;
				if (m_Layout)
				{
					size = m_Layout->GetPreferredSize(this);
				}
				else
				{
					size = m_Size;
				}

				if (m_FitText)
				{
					auto ps = UIElementInterface::GetPreferredSize();
					size = glm::vec2(
						std::max(ps.x, (float)m_Text->GetPixelWidth() + 8),
						std::max(ps.y, (float)m_Text->GetPixelHeight())
					);
				}

				return size;
			}

			virtual void Update(Timestep ts)
			{
				m_Counter += ts;
				if (m_CursorInterval < m_Counter)
				{
					m_DrawCursor = !m_DrawCursor;
					m_Counter = 0.f;
				}

				Panel::Update(ts);
			}

			void inline UpdateCursorPos()
			{
				auto layout = std::static_pointer_cast<BoxLayout>(m_Layout);
				if (m_InsertPos > 0)
				{
					auto ch = m_Text->GetCharacterRect(m_InsertPos);
					m_CursorPos = glm::ivec2(layout->GetMargin() + ch.z, layout->GetMargin());
				}
				else
				{
					m_CursorPos = glm::ivec2(layout->GetMargin());
				}
			}

			void StartSelection()
			{
				m_SelectionStart = m_InsertPos;
				m_SelectionEnd = m_InsertPos;
				m_DrawSelectionBox = true;
			}

			void StopSelection()
			{
				m_SelectionStart = -1;
				m_DrawSelectionBox = false;
			}

			bool IsSelecting() const { return m_SelectionStart != -1; }
			void UpdateSelection();

		protected:
			void HandleDirectionalKeys(KeyPressedEvent& e);
			void HandleCommandKeys(KeyPressedEvent& e);
			void HandleCharacterKeys(CharacterEvent& e);

			virtual bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

		private:
			Ref<CharacterList> m_Text;
			Ref<TextBox> m_TextBox;

			bool m_FitText = true;
			bool m_DrawCursor = true;
			bool m_DrawSelectionBox = false;
			bool m_MouseDown = false;

			float m_CursorInterval = 0.5f;
			float m_Counter = 0;

			size_t m_InsertPos;
			size_t m_SelectionStart;
			size_t m_SelectionEnd;

			glm::ivec2 m_CursorPos;
		};
	}
}
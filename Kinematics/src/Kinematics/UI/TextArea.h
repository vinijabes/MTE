#pragma once
#include "ScrollPanel.h"
#include "TextBox.h"

namespace Kinematics {
	namespace UI {
		class TextArea;

		class TextLine : public Panel
		{
		public:
			friend TextArea;

			TextLine();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void SetLayout(Ref<Layout> layout) final {};

			void SetSelection(glm::uvec2 selection);
			void SetSelection(uint32_t start, uint32_t end);

			glm::vec2 GetCharacterPosition(uint32_t pos) const;
			glm::vec2 GetCharacterAbsolutePosition(uint32_t pos) const;

			glm::vec4 GetCharacterRect(uint32_t pos) const;
			int GetCharPosFromXCoords(float pos) const ;

			size_t GetLength() const;

			void SetText(const std::string& text);
			void SetFontColor(const glm::vec4& color);
			std::string GetSelectedString();
		protected:
			void UpdateSelectionBox();
		private:
			glm::uvec2 m_Selection;

			Ref<Panel> m_SelectionBox;
			Ref<TextBox> m_TextBox;

			Ref<CharacterList> m_String;
		};

		class TextArea : public ScrollPanel
		{
		public:
			TextArea();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void Update(Timestep ts);

			void SetSelection(glm::uvec4 selection);
			void SetSelection(uint32_t startLine, uint32_t endLine, uint32_t startPos, uint32_t endPos);
			void SetSelectionStart(uint32_t startLine, uint32_t startPos);
			void SetSelectionEnd(uint32_t endLine, uint32_t endPos);
			bool IsSelecting();

			void UpdateLinesSelection();
			void PushBackLine();
			void PushLine();

			void HandleCharacterInput(CharacterEvent& e);
			void HandleCommands(KeyPressedEvent& e);
			void HandleDirectionalKeys(KeyPressedEvent& e);
			void HandleMouseClick(MouseButtonReleasedEvent& e);

			void SetCursorPosition(glm::uvec2 pos);
			void SetCursorXPosition(uint32_t x);
			void SetCursorYPosition(uint32_t y);

			glm::uvec2 GetCursorPositionFromAbsoluteCoords(glm::vec2 coords);
			glm::uvec2 GetCursorPositionFromCoords(glm::vec2 coords);
			glm::vec2  GetCursorCoords();
		protected:
			void UpdateCursorPanelPosition();
			void StopSelection();
		private:
			glm::uvec2 m_Cursor;
			glm::uvec4 m_Selection;
			std::vector<Ref<TextLine>> m_Lines;

			Ref<Panel> m_CursorPanel;
		};
	}
}
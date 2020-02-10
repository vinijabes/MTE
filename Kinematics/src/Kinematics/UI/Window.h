#pragma once

#include "Panel.h"
#include "TextBox.h"

namespace Kinematics {
	namespace UI {
		class WindowHeader : public Panel
		{
		public:
			WindowHeader();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void Update(Timestep ts) override;

			void SetTitle(const std::string& title);
			Ref<TextBox> GetTitle();

			virtual void PushChild(Ref<UIElementInterface> child) { m_Header->PushChild(child); }
			
			void SetTitleFont(const Ref<FontFace>& font);

		private:
			Ref<Panel> m_Header;
			Ref<TextBox> m_Text;
		};

		class Window : public UIElementInterface
		{
		public:
			Window();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void Update(Timestep ts) override;
			virtual void PushChild(Ref<UIElementInterface> child) { m_Body->PushChild(child); }

			void PushHeaderChild(Ref<UIElementInterface> child) { m_Header->PushChild(child); }

			void SetBodyLayout(Ref<Layout> layout) { m_Body->SetLayout(layout); }
			void SetTitle(const std::string& title);
			void SetTitleFont(const Ref<FontFace>& font);

			void SetFullSize(bool full) { m_FullSize = full; }

			void UpdateFocus(Ref<UIElementInterface> element);

		private:
			Ref<Panel> m_Body;
			Ref<WindowHeader> m_Header;

			std::vector<Ref<UIElementInterface>> m_FocusPath;

			bool m_FullSize = false;
		};
	}
}
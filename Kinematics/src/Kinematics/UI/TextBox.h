#pragma once

#include "UIElementInterface.h"
#include "Kinematics/Renderer/Text.h"

#include "Kinematics/Framework/Managers/ResourceManager.h"

namespace Kinematics {
	namespace UI {

		class TextBox : public UIElementInterface
		{
		public:
			TextBox();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void Update(Timestep ts) override;
			virtual glm::vec2 GetPreferredSize() const override;


			void SetText(Ref<Text> text) { m_Text = text; }
			void SetText(const std::string& text) 
			{
				if (!m_Text)
				{
					m_Text = CreateRef<Text>(FontFace::DEFAULT);
				}

				m_Text->SetText(text); 
			}

			void RecalculateSize();

			virtual void SetLayout(Ref<Layout> layout) final {}

			Ref<Text> GetText() { return m_Text; }

		private:
			Ref<Text> m_Text;
		};
	}
}
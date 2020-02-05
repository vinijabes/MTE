#pragma once

#include "Panel.h"

namespace Kinematics {
	namespace UI {
		class Window : public UIElementInterface
		{
		public:
			Window();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
			virtual void PushChild(Ref<UIElementInterface> child) { m_Body->PushChild(child); }

			void SetBodyLayout(Ref<Layout> layout) { m_Body->SetLayout(layout); }
		private:
			Ref<Panel> m_Body;
		};
	}
}
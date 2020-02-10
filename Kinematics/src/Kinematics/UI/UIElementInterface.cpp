#include "mtepch.h"
#include "UIElementInterface.h"

#include "Window.h"

namespace Kinematics {
	namespace UI {
		void UIElementInterface::RequestFocus()
		{
			GetWindow()->UpdateFocus(GetPtr());
		}

		Ref<Window> UIElementInterface::GetWindow()
		{
			if (dynamic_cast<Window*>(this) != nullptr) return std::dynamic_pointer_cast<Window>(GetPtr());

			auto parent = m_Parent;
			while (parent)
			{
				if (dynamic_cast<Window*>(parent) != nullptr) break;
				parent = parent->m_Parent;
			}

			return std::dynamic_pointer_cast<Window>(parent->GetPtr());
		}

		bool UIElementInterface::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
		{
			auto mousePos = RenderCommand::GetWindow()->GetMousePos();

			if (m_Children.size() > 0)
			{
				bool insideChild = false;
				for (auto c : m_Children)
				{
					if (c->InsideElement(mousePos)) insideChild = true;
				}

				if (!insideChild && InsideElement(mousePos))
					GetWindow()->UpdateFocus(GetPtr());
			}
			else
			{
				if (InsideElement(mousePos))
					GetWindow()->UpdateFocus(GetPtr());
			}

			if (InsideElement(mousePos))
				m_OnClick(e);
			return e.Handled;
		}
	}
}

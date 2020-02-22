#include "mtepch.h"
#include "UIElementInterface.h"

#include "Window.h"

namespace Kinematics {
	namespace UI {
		void UIElementInterface::PushChild(Ref<UIElementInterface> child)
		{
			m_Children.push_back(child);
			child->SetParent(this);

			if (m_Theme)
				child->SetTheme(m_Theme);

			auto window = GetWindow();
			if (window)
				window->ApplyLayout();
			else
				ApplyLayout();
		}

		void UIElementInterface::PushChild(Ref<UIElementInterface> child, uint32_t pos)
		{
			m_Children.insert(m_Children.begin() + pos, child);
			child->SetParent(this);

			if (m_Theme)
				child->SetTheme(m_Theme);

			auto window = GetWindow();
			if (window)
				window->ApplyLayout();
			else
				ApplyLayout();
		}

		void UIElementInterface::RemoveChild(Ref<UIElementInterface> child)
		{
			for (auto c = m_Children.cbegin(); c != m_Children.cend(); c++)
			{
				if (*c == child)
				{
					m_Children.erase(c);
					return;
				}
			}

			auto window = GetWindow();
			if (window)
				window->ApplyLayout();
			else
				ApplyLayout();
		}

		void UIElementInterface::RequestFocus()
		{
			GetWindow()->UpdateFocus(GetPtr());
		}

		void UIElementInterface::SetFixedSize(const glm::vec2& fixedSize)
		{
			if (m_FixedSize != fixedSize)
			{
				m_FixedSize = fixedSize;
				auto window = GetWindow();
				if (window)
					window->ApplyLayout();
				else
					ApplyLayout();
			}
		}

		void UIElementInterface::SetFixedPosition(const glm::vec2 position)
		{ 
			if (m_FixedPosition != position)
			{
				m_FixedPosition = position; 
				auto window = GetWindow();
				if (window)
					window->ApplyLayout();
				else
					ApplyLayout();
			}
		}

		void UIElementInterface::SetWeight(const glm::vec2 weight)
		{ 
			if (m_Weight != weight)
			{
				m_Weight = weight; 
				auto window = GetWindow();
				if (window)
					window->ApplyLayout();
				else
					ApplyLayout();
			}
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

			if (parent && dynamic_cast<Window*>(parent))
				return std::dynamic_pointer_cast<Window>(parent->GetPtr());
			return nullptr;
		}

		bool UIElementInterface::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
		{
			auto mousePos = RenderCommand::GetWindow()->GetMousePos();

			if (InsideElement(mousePos))
				m_OnClick(e);
			return e.Handled;
		}

		bool UIElementInterface::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
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
				m_OnButtonDown(e);

			return e.Handled;
		}

		bool UIElementInterface::OnMouseScrolledEvent(MouseScrolledEvent& e)
		{
			auto mousePos = RenderCommand::GetWindow()->GetMousePos();
			if (InsideElement(mousePos))
				m_OnScroll(e);

			return e.Handled;
		}
		bool UIElementInterface::OnMouseMovedEvent(MouseMovedEvent& e)
		{
			if (InsideElement(e.GetX(), e.GetY()) || (IsFocused() && RenderCommand::GetWindow()->GetButtonState(KINEMATICS_MOUSE_BUTTON_LEFT)))
			{
				if (m_MouseInside == false)
				{
					m_OnEnter(e);
					m_MouseInside = true;
				}
				m_OnMove(e);
			}
			else
			{
				if (m_MouseInside)
				{
					m_OnOut(e);
					m_MouseInside = false;
				}
			}

			return e.Handled;
		}
	}
}

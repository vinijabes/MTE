#pragma once

#include "Kinematics/Renderer/RenderCommand.h"
#include "Kinematics/Renderer/Renderer2D.h"

#include "Kinematics/Framework/Events/ApplicationEvent.h"
#include "Kinematics/Framework/Events/MouseEvent.h"
#include "Kinematics/Framework/Events/KeyEvent.h"

#include "Kinematics/Scripting/Callback.h"

#include <vector>

#include "UIConstraint.h"
#include "Layout.h"
#include "Theme.h"
#include "Kinematics/Renderer/Camera.h"

namespace Kinematics {
	namespace UI {

		class Window;

		class UIElementInterface : public std::enable_shared_from_this<UIElementInterface>
		{
		public:
			UIElementInterface()
				: m_Parent(nullptr), m_Size(0.f), m_FixedSize(0.f), m_MinSize(0.f), m_Position(0.f), m_Transformation(1.f), m_Weight(0.f),
				m_Enabled(true), m_Visible(true), m_Focused(false), m_Layout(nullptr)
			{}

			Ref<UIElementInterface> GetPtr() { return shared_from_this(); }

			glm::mat4 GetTransformation() const { return m_Transformation; }

			virtual void Draw(Camera& camera) { Draw(camera, glm::vec2(0)); };
			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) = 0;

			virtual void Update(Timestep ts) { UpdateChildren(ts); };
			virtual void OnChange() {};

			virtual void OnEvent(Event& e)
			{
				NotifyEventChildren(e);
				EventDispatcher dispatcher(e);

				dispatcher.Dispatch<MouseButtonReleasedEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnMouseButtonReleasedEvent));
				dispatcher.Dispatch<MouseButtonPressedEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnMouseButtonPressedEvent));
				dispatcher.Dispatch<KeyPressedEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnKeyPressedEvent));
				dispatcher.Dispatch<CharacterEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnCharTypedEvent));
				dispatcher.Dispatch<MouseMovedEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnMouseMovedEvent));
				dispatcher.Dispatch<MouseScrolledEvent>(KINEMATICS_BIND_EVENT_FN(UIElementInterface::OnMouseScrolledEvent));
			}

			void NotifyChange()
			{
				RecalculateTransformation();
				if (m_Parent)
					m_Parent->NotifyChange();
				else if (m_Layout)
					m_Layout->ApplyLayout(this);
			}

			void RecalculateTransformation()
			{
			}

			virtual void PushChild(Ref<UIElementInterface> child)
			{
				m_Children.push_back(child);
				child->SetParent(this);

				if (m_Theme)
					child->SetTheme(m_Theme);
			}

			virtual void PushChild(Ref<UIElementInterface> child, uint32_t pos)
			{
				m_Children.insert(m_Children.begin() + pos, child);
				child->SetParent(this);

				if (m_Theme)
					child->SetTheme(m_Theme);
			}

			virtual void RemoveChild(Ref<UIElementInterface> child)
			{
				for (auto c = m_Children.cbegin(); c != m_Children.cend(); c++)
				{
					if (*c == child)
					{
						m_Children.erase(c);
						return;
					}
				}
			}

			const std::vector<Ref<UIElementInterface>> &GetChildren() const { return m_Children; }

			void SetParent(UIElementInterface* parent)
			{
				m_Parent = parent;
			}

			UIElementInterface* GetParent() { return m_Parent; }

			bool IsFocused() { return m_Focused; }
			void SetFocus(bool focus)
			{
				if (m_Focused != focus)
				{
					if (focus) m_OnFocus();
					else m_OnUnfocus();

					m_Focused = focus;
				}
			}

			void RequestFocus();

			bool IsEnabled() { return m_Enabled; }
			bool IsVisible() { return m_Visible; }

			void Disable() { m_Enabled = false; }
			void Enable() { m_Enabled = true; }

			virtual glm::vec2 GetFixedSize() const { return m_FixedSize; }
			virtual void SetFixedSize(const glm::vec2& fixedSize) { m_FixedSize = fixedSize; }

			virtual glm::vec2 GetSize() const { return m_Size; }
			virtual void SetSize(const glm::vec2 size)
			{ 
				m_Size = size; 
				m_OnResize(size);
			}

			virtual glm::vec2 GetMinSize() { return m_MinSize; }
			virtual void SetMinSize(const glm::vec2 size) { m_MinSize = size; }

			virtual glm::vec2 GetPosition() { return m_Position; }
			virtual void SetPosition(const glm::vec2 position) { m_Position = position; }

			virtual glm::vec2 GetWeight() { return m_Weight; }
			virtual void SetWeight(const glm::vec2 weight) { m_Weight = weight; }

			virtual glm::vec2 GetAbsolutePosition() const
			{
				if (m_Parent)
				{
					auto parent = m_Parent;
					glm::vec2 pos = m_Position;

					while (parent)
					{
						pos += parent->m_Position;
						parent = parent->m_Parent;
					}

					return pos;
				}

				return m_Position;
			}


			virtual glm::vec2 GetPreferredSize() const
			{
				if (m_Layout)
				{
					return m_Layout->GetPreferredSize(this);
				}
				else
				{
					auto size = GetSize();
					return glm::vec2(std::max(m_MinSize.x, size.x), std::max(m_MinSize.y, size.y));
				}
			}

			virtual void SetLayout(Ref<Layout> layout) { m_Layout = layout; }
			Ref<Layout> GetLayout() const { return m_Layout; }
			virtual void ApplyLayout()
			{
				if (m_Layout)
				{
					m_Layout->ApplyLayout(this);
				}
				else
				{
					for (auto c : m_Children)
					{
						glm::vec2 preferredSize = c->GetPreferredSize();
						glm::vec2 fixedSize = c->GetFixedSize();

						c->SetSize(glm::vec2(
							fixedSize.x ? fixedSize.x : preferredSize.x,
							fixedSize.y ? fixedSize.y : preferredSize.y
						));

						c->ApplyLayout();
					}
				}
			}

			void SetTheme(Ref<Theme> theme)
			{
				m_Theme = theme;
				for (auto c : m_Children) c->SetTheme(theme);
			}

			Ref<Window> GetWindow();

		protected:
			void NotifyEventChildren(Event& e) { for (auto child : m_Children) child->OnEvent(e); }
			void DrawChildren(Camera& camera, glm::vec2 parentPos) { for (auto child : m_Children) child->Draw(camera, parentPos); }

			void UpdateChildren(Timestep ts) { for (auto child : m_Children) child->Update(ts); }

			virtual bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
			virtual bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
			virtual bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		private:
			

			bool InsideElement(double x, double y)
			{
				auto pos = GetAbsolutePosition();

				if (std::abs(x - pos.x - m_Size.x / 2) < m_Size.x / 2 && std::abs(y - pos.y - m_Size.y / 2) < m_Size.y / 2) return true;
				return false;
			}

			bool InsideElement(glm::vec2 pos)
			{
				return InsideElement(pos.x, pos.y);
			}

			bool OnMouseMovedEvent(MouseMovedEvent& e)
			{
				if (InsideElement(e.GetX(), e.GetY()))
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

			bool OnKeyPressedEvent(KeyPressedEvent& e)
			{
				m_OnInput(e);
				return e.Handled;
			}

			bool OnCharTypedEvent(CharacterEvent& e)
			{
				m_OnChar(e);
				return e.Handled;
			}

		protected:
			glm::mat4 m_Transformation;

			glm::vec2 m_Position;

			glm::vec2 m_Size;
			glm::vec2 m_MinSize;
			glm::vec2 m_FixedSize;
			glm::vec2 m_Weight;

			UIElementInterface* m_Parent;
			std::vector<Ref<UIElementInterface>> m_Children;

			Ref<Layout> m_Layout;
			Ref<Theme> m_Theme;

			bool m_Visible;

			bool m_Enabled;
			bool m_Focused;

			Callback<void, glm::vec2> m_OnResize;

			Callback<void, MouseButtonReleasedEvent&> m_OnClick;
			Callback<void, MouseButtonPressedEvent&> m_OnButtonDown;
			Callback<void, MouseScrolledEvent&> m_OnScroll;

			Callback<void, KeyPressedEvent&> m_OnInput;
			Callback<void, CharacterEvent&> m_OnChar;

			bool m_MouseInside = false;
			Callback<void, MouseMovedEvent&> m_OnEnter;
			Callback<void, MouseMovedEvent&> m_OnOut;
			Callback<void, MouseMovedEvent&> m_OnMove;

			Callback<void> m_OnFocus;
			Callback<void> m_OnUnfocus;
		};
	}
}
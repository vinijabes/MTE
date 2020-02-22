#include "mtepch.h"
#include "ScrollPanel.h"

#include "Window.h"

namespace Kinematics {
	namespace UI {
		ScrollPanel::ScrollPanel()
			: m_ScrollPosition(0.f)
		{
			auto flexLayout = CreateRef<FlexLayout>();
			flexLayout->SetJustify(Justify::FlexStart);

			UIElementInterface::SetLayout(flexLayout);

			m_OnScroll = m_OnScroll + CreateRef<FunctionCallback<void, MouseScrolledEvent&>>([this](MouseScrolledEvent& e) {
				m_ScrollPosition.y -= e.GetYOffset() * 10;
				m_ScrollPosition.x -= e.GetXOffset() * 10;
				SetScrollPosition(m_ScrollPosition);

				e.StopPropagation();
			});

			m_ContainerPanel = CreateRef<ScrollContainer>();
			m_VerticalScroll = CreateRef<ScrollBar>(Orientation::Vertical, m_ContainerPanel);
			m_HorizontalScroll = CreateRef<ScrollBar>(Orientation::Horizontal, m_ContainerPanel);

			auto box = Kinematics::CreateRef<BoxLayout>();
			box->SetMargin(0.f);
			box->SetAlignment(Alignment::Minimum);

			m_ContainerPanel->SetLayout(box);

			m_VerticalScroll->SetFixedSize({ 15.f, 10.f });
			m_VerticalScroll->SetFixedPosition(glm::vec2(m_ContainerPanel->GetSize().x, 0.f));
			m_VerticalScroll->SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

			m_HorizontalScroll->SetFixedSize({ 10.f, 15.f });
			m_HorizontalScroll->SetFixedPosition(glm::vec2(0.f, m_ContainerPanel->GetSize().y));
			m_HorizontalScroll->SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

			m_OnResize = m_OnResize + CreateRef<FunctionCallback<void, glm::vec2>>([this](glm::vec2 size) {
				UpdateContainerSize();
			});

			m_OnButtonDown = m_OnButtonDown + CreateRef<FunctionCallback<void, MouseButtonPressedEvent&>>([this](MouseButtonPressedEvent& e) {
				m_StartScrollPosition = m_ScrollPosition;
				m_StartMousePos = RenderCommand::GetWindow()->GetMousePos();
			});

			m_HorizontalScroll->m_Scroll->AddOnMoveCallback(CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e){
				if (m_HorizontalScroll->m_Scroll->IsFocused())
				{
					auto size = (m_HorizontalScroll->GetSize() - m_HorizontalScroll->m_Scroll->GetSize());
					if (RenderCommand::GetWindow()->GetButtonState(KINEMATICS_MOUSE_BUTTON_LEFT))
					{
						auto move = glm::vec2((e.GetX() - m_StartMousePos.x) * m_HorizontalScroll->GetPercentagePixelValue() * 100.f / size.x, 0.f);
						SetScrollPosition(m_StartScrollPosition + move);
					}
				}
			}));

			m_VerticalScroll->m_Scroll->AddOnMoveCallback(CreateRef<FunctionCallback<void, MouseMovedEvent&>>([this](MouseMovedEvent& e) {
				if (m_VerticalScroll->m_Scroll->IsFocused())
				{
					auto size = (m_VerticalScroll->GetSize() - m_VerticalScroll->m_Scroll->GetSize());
					if (RenderCommand::GetWindow()->GetButtonState(KINEMATICS_MOUSE_BUTTON_LEFT))
					{
						auto move = glm::vec2(0.f, (e.GetY() - m_StartMousePos.y) * m_VerticalScroll->GetPercentagePixelValue() * 100.f / size.y);
						SetScrollPosition(m_StartScrollPosition + move);
					}
				}
			}));

			UIElementInterface::PushChild(m_ContainerPanel);
			UIElementInterface::PushChild(m_HorizontalScroll);
			UIElementInterface::PushChild(m_VerticalScroll);
		}

		void ScrollPanel::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto window = RenderCommand::GetWindow();
			RenderCommand::SetScissor(
				std::clamp(parentPos.x + m_Position.x - 1, 0.f, m_ContainerPanel->GetSize().x + 1),
				window->GetHeight() - parentPos.y - m_Position.y - m_ContainerPanel->GetSize().y,
				m_ContainerPanel->GetSize().x + 1,
				m_ContainerPanel->GetSize().y
			);
			RenderCommand::EnableScissorTest();
			m_ContainerPanel->Draw(camera, parentPos + m_Position, m_ScrollPosition);
			RenderCommand::DisableScissorTest();
			RenderCommand::SetScissor(0, 0, window->GetWidth(), window->GetHeight());

			if(m_VerticalScroll->IsVisible())
				m_VerticalScroll->Draw(camera, parentPos + m_Position);

			if (m_HorizontalScroll->IsVisible())
				m_HorizontalScroll->Draw(camera, parentPos + m_Position);
		}

		void ScrollPanel::Update(Timestep ts)
		{
			UpdateChildren(ts);
			m_VerticalScroll->Update(ts);
			m_HorizontalScroll->Update(ts);

			auto overflow = m_ContainerPanel->GetPreferredSize() - m_ContainerPanel->GetSize();

			m_HorizontalScroll->SetVisibility(overflow.x > 0.f);
			m_VerticalScroll->SetVisibility(overflow.y > 0.f);

			m_VerticalScroll->SetFixedPosition(glm::vec2(m_ContainerPanel->GetSize().x, 0.f));
			m_HorizontalScroll->SetFixedPosition(glm::vec2(0.f, m_ContainerPanel->GetSize().y));
		}

		void ScrollPanel::OnEvent(Event& e)
		{
			m_VerticalScroll->OnEvent(e);
			m_HorizontalScroll->OnEvent(e);
			UIElementInterface::OnEvent(e);
		}

		void ScrollPanel::UpdateContainerSize()
		{
			if (m_FixedSize != glm::vec2(0.f))
			{
				auto ps = m_ContainerPanel->GetPreferredSize();
				auto overflow = ps - m_Size;
				if (m_Size.x < 0.f || m_Size.y < 0.f) overflow = glm::vec2(0.f);

				auto ScrollOffset = glm::vec2(0.f, 0.f);
				if (overflow.y > 0.f)
				{
					m_VerticalScroll->SetVisibility(true);
					ScrollOffset.x = m_VerticalScroll->GetSize().x;
				}
				else
				{
					m_VerticalScroll->SetVisibility(false);
				}

				if (overflow.x > 0.f)
				{
					m_HorizontalScroll->SetVisibility(true);
					ScrollOffset.y = m_HorizontalScroll->GetSize().y;
				}
				else
				{
					m_HorizontalScroll->SetVisibility(false);
				}

				m_VerticalScroll->SetFixedSize(glm::vec2(m_VerticalScroll->GetFixedSize().x, m_Size.y - m_HorizontalScroll->IsVisible() * m_HorizontalScroll->GetFixedSize().y));
				m_HorizontalScroll->SetFixedSize(glm::vec2(m_Size.x - m_VerticalScroll->IsVisible() * m_VerticalScroll->GetFixedSize().x, m_HorizontalScroll->GetFixedSize().y));

				m_ContainerPanel->SetFixedSize(m_Size - ScrollOffset);
				SetScrollPosition(glm::vec2(
					overflow.x * m_HorizontalScroll->GetPercentage(),
					overflow.y * m_VerticalScroll->GetPercentage()
				));
			}
			else
			{
				m_VerticalScroll->SetSize(glm::vec2(0.f));
				m_HorizontalScroll->SetSize(glm::vec2(0.f));

				m_ContainerPanel->SetSize(m_Size);
			}
		}

		void ScrollPanel::SetScrollPosition(glm::vec2 pos)
		{
			auto layout = std::static_pointer_cast<BoxLayout>(m_ContainerPanel->GetLayout());
			m_ScrollPosition = pos;

			auto overflow = m_ContainerPanel->GetPreferredSize() - m_ContainerPanel->GetSize();
			if (m_ScrollPosition.y > overflow.y&& overflow.y > 0) m_ScrollPosition.y = overflow.y;
			if (m_ScrollPosition.x > overflow.x&& overflow.x > 0) m_ScrollPosition.x = overflow.x;

			if (m_ScrollPosition.y < 0) m_ScrollPosition.y = 0;
			if (m_ScrollPosition.x < 0) m_ScrollPosition.x = 0;

			if (overflow.x > 0.f) m_HorizontalScroll->SetPercentage(m_ScrollPosition.x / overflow.x);
			if (overflow.y - layout->GetMargin() > 0.f) m_VerticalScroll->SetPercentage(m_ScrollPosition.y/(overflow.y - layout->GetMargin()));
		}

		ScrollPanel::ScrollBar::ScrollBar(Orientation orientation, Ref<ScrollContainer> container)
			: m_Orientation(orientation), m_ContainerPanel(container), m_Percentage(0.f), m_LastPercentage(0.f)
		{
			auto size = glm::vec2(0.f);
			size[(int)orientation] = 15.f;

			auto panel = CreateRef<Panel>();
			panel->SetSize(size);
			panel->SetColor(glm::vec4(1.f));
			panel->SetHighlightColor(glm::vec4(0.f, 1.f, 1.f, 1.f));

			m_OnButtonDown = m_OnButtonDown + CreateRef<FunctionCallback<void, MouseButtonPressedEvent&>>([this](MouseButtonPressedEvent& e) {
				m_LastPercentage = m_Percentage;
			});

			m_OnFocus = m_OnFocus + CreateRef<FunctionCallback<void>>([this]() {
				KINEMATICS_TRACE("FOCUS GAIN");
			});

			m_OnUnfocus = m_OnUnfocus + CreateRef<FunctionCallback<void>>([this]() {
				KINEMATICS_TRACE("FOCUS LOST");
				auto window = GetWindow();
				window->GetSize();
			});

			m_Scroll = panel;
			PushChild(m_Scroll);
		}

		void ScrollPanel::ScrollBar::Draw(Camera& camera, glm::vec2 pos)
		{
			Panel::Draw(camera, pos);
		}

		void ScrollPanel::ScrollBar::Update(Timestep ts)
		{
			ResizeScroll();
			UpdateScrollPosition();
		}

		bool ScrollPanel::ScrollBar::OnMouseMovedEvent(MouseMovedEvent& e)
		{
			m_Scroll->FireOnMoveEvent(e);
			return Panel::OnMouseMovedEvent(e);
		}

		void ScrollPanel::ScrollContainer::Draw(Camera& camera, glm::vec2 parentPos, glm::vec2 scroll)
		{
			if (m_Color != glm::vec4(0.f))
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Color);
			}
			else if (m_Theme)
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Theme->m_Color);
			}

			DrawChildren(camera, parentPos + m_Position - scroll);
		}
		void ScrollPanel::ScrollContainer::SetFixedSize(const glm::vec2& size)
		{
			if (m_FixedSize != size)
			{
				m_FixedSize = size;
				ApplyLayout();
			}
		}
	}
}
#include "mtepch.h"
#include "ScrollPanel.h"

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

			m_VerticalScroll = CreateRef<ScrollBar>(Orientation::Vertical);
			m_HorizontalScroll = CreateRef<ScrollBar>(Orientation::Horizontal);
			m_ContainerPanel = CreateRef<ScrollContainer>();

			m_VerticalScroll->SetParent(m_ContainerPanel.get());
			m_HorizontalScroll->SetParent(m_ContainerPanel.get());

			m_VerticalScroll->SetSize({ 15.f, 10.f });
			m_VerticalScroll->SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

			m_HorizontalScroll->SetSize({ 10.f, 15.f });
			m_HorizontalScroll->SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

			m_OnResize = m_OnResize + CreateRef<FunctionCallback<void, glm::vec2>>([this](glm::vec2 size) {
				m_VerticalScroll->SetSize(glm::vec2(m_VerticalScroll->GetSize().x, size.y - m_HorizontalScroll->GetSize().y));
				m_HorizontalScroll->SetSize(glm::vec2(size.x - m_VerticalScroll->GetSize().x, m_HorizontalScroll->GetSize().y));

				m_ContainerPanel->SetFixedSize(m_Size - glm::vec2(m_VerticalScroll->GetSize().x, m_HorizontalScroll->GetSize().y));
				});

			UIElementInterface::PushChild(m_ContainerPanel);
		}

		void ScrollPanel::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto window = RenderCommand::GetWindow();
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

			RenderCommand::SetScissor(
				parentPos.x + m_Position.x - 1,
				window->GetHeight() - parentPos.y - m_Position.y - m_ContainerPanel->GetSize().y,
				m_ContainerPanel->GetSize().x + 1,
				m_ContainerPanel->GetSize().y
			);
			RenderCommand::EnableScissorTest();
			m_ContainerPanel->Draw(camera, parentPos + m_Position, m_ScrollPosition);
			RenderCommand::DisableScissorTest();
			RenderCommand::SetScissor(0, 0, window->GetWidth(), window->GetHeight());

			m_VerticalScroll->Draw(camera, parentPos + m_Position + glm::vec2(m_ContainerPanel->GetSize().x, 0.f));
			m_HorizontalScroll->Draw(camera, parentPos + m_Position + glm::vec2(0.f, m_ContainerPanel->GetSize().y));
		}

		void ScrollPanel::Update(Timestep ts)
		{
			UpdateChildren(ts);
			m_VerticalScroll->Update(ts);
			m_HorizontalScroll->Update(ts);
		}

		void ScrollPanel::SetScrollPosition(glm::vec2 pos)
		{
			auto layout = std::static_pointer_cast<BoxLayout>(m_ContainerPanel->GetLayout());
			m_ScrollPosition = pos;

			auto overflow = m_ContainerPanel->GetPreferredSize() - m_ContainerPanel->GetSize();
			if (m_ScrollPosition.y > overflow.y&& overflow.y > 0) m_ScrollPosition.y = overflow.y;
			if (m_ScrollPosition.x > overflow.x&& overflow.x > 0) m_ScrollPosition.x = overflow.x;

			if (m_ScrollPosition.y < 0) m_ScrollPosition.y = 0;
			//if (m_ScrollPosition.x < 0) m_ScrollPosition.x = 0;

			if (overflow.x > 0.f) m_HorizontalScroll->SetPercentage(m_ScrollPosition.x / overflow.x);
			if (overflow.y - layout->GetMargin() > 0.f) m_VerticalScroll->SetPercentage(m_ScrollPosition.y/(overflow.y - layout->GetMargin()));
		}

		ScrollPanel::ScrollBar::ScrollBar(Orientation orientation)
			: m_Orientation(orientation), m_Percentage(0.f)
		{
			auto size = glm::vec2(0.f);
			size[(int)orientation] = 15.f;

			auto panel = CreateRef<Panel>();
			panel->SetSize(size);
			panel->SetColor(glm::vec4(1.f));

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
	}
}
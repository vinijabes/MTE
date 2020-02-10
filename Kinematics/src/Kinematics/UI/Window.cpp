#include "mtepch.h"
#include "Window.h"

#include "Kinematics/Framework/Managers/ResourceManager.h"

namespace Kinematics {
	namespace UI {
		Window::Window()
		{
			m_Body = CreateRef<Panel>();
			m_Header = CreateRef<WindowHeader>();

			auto layout = CreateRef<BoxLayout>();
			auto headerLayout = CreateRef<BoxLayout>();
			auto windowLayout = CreateRef<BoxLayout>();

			layout->SetOrientation(Orientation::Vertical);
			layout->SetAlignment(Alignment::Fill);

			windowLayout->SetOrientation(Orientation::Vertical);
			windowLayout->SetAlignment(Alignment::Fill);
			windowLayout->SetMargin(0);

			headerLayout->SetOrientation(Orientation::Vertical);
			headerLayout->SetAlignment(Alignment::Fill);
			headerLayout->SetMargin(0);

			this->SetLayout(windowLayout);
			m_Header->SetLayout(headerLayout);
			m_Body->SetLayout(layout);

			UIElementInterface::PushChild(m_Header);
			UIElementInterface::PushChild(m_Body);
		}

		void Window::Draw(Camera& camera, glm::vec2 parentPos)
		{
			m_Header->Draw(camera, m_Position);
			m_Body->Draw(camera, m_Position);
		}

		void Window::SetTitle(const std::string& title)
		{
			m_Header->SetTitle(title);
		}

		void Window::SetTitleFont(const Ref<FontFace>& font)
		{
			m_Header->SetTitleFont(font);
		}

		void Window::UpdateFocus(Ref<UIElementInterface> element)
		{
			for (auto e : m_FocusPath)
			{
				if (!e->IsFocused())
					continue;

				e->SetFocus(false);
			}

			m_FocusPath.clear();

			Ref<Window> window = Ref<Window>(nullptr);

			while (element)
			{
				m_FocusPath.push_back(element);
				if (std::dynamic_pointer_cast<Window>(element))
					window = std::dynamic_pointer_cast<Window>(element);

				if (element->GetParent())
					element = element->GetParent()->GetPtr();
				else
					break;
			}

			for (auto it = m_FocusPath.rbegin(); it != m_FocusPath.rend(); ++it)
				(*it)->SetFocus(true);

		}

		void Window::Update(Timestep ts)
		{
			if(!m_Parent)
				m_Size = glm::vec2(m_Weight.x * RenderCommand::GetWindow()->GetWidth(), m_Weight.y * RenderCommand::GetWindow()->GetHeight());
			m_Body->SetSize(glm::vec2(m_Size.x, m_Size.y - m_Header->GetSize().y));
			UpdateChildren(ts);
		}

		WindowHeader::WindowHeader()
		{
			m_Header = CreateRef <Panel>();
			m_Text = CreateRef<TextBox>();

			auto headerLayout = CreateRef<FlexLayout>();
			headerLayout->SetJustify(Justify::Center);

			m_Header->PushChild(m_Text);

			m_Header->SetLayout(headerLayout);
			m_Header->SetWeight({ 1.f, 1.f });
			
			UIElementInterface::PushChild(m_Header);
		}

		void WindowHeader::SetTitle(const std::string& title)
		{
			m_Text->SetText(title);
		}

		Ref<TextBox> WindowHeader::GetTitle()
		{
			return m_Text;
		}

		void WindowHeader::SetTitleFont(const Ref<FontFace>& font)
		{
			std::string title = "";

			if (m_Text->GetText())
				title = m_Text->GetText()->GetRawText();

			auto text = CreateRef<Text>(font);
			text->SetText(title);

			m_Text->SetText(text);
		}

		void WindowHeader::Draw(Camera& camera, glm::vec2 parentPos)
		{
			auto pos = m_Position;
			auto size = m_Size;
			auto drawingPos = pos + size / 2.f + parentPos;
			
			/*if(m_Theme)
				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Theme->m_WindowHeaderColor);*/
			
			DrawChildren(camera, m_Position);
		}

		void WindowHeader::Update(Timestep ts)
		{
			if (m_Theme)
				m_Header->SetColor(m_Theme->m_WindowHeaderColor);
			
			UpdateChildren(ts);
		}
	}
}
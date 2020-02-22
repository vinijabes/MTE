#include "mtepch.h"
#include "Layout.h"

#include "UIElementInterface.h"

namespace Kinematics
{
	namespace UI {
		glm::vec2 BoxLayout::GetPreferredSize(const UIElementInterface* element) const
		{
			glm::vec2 size = glm::vec2(2 * m_Margin);

			bool first = true;

			int mainAxis = (int)m_Orientation;
			int secondaryAxis = (mainAxis + 1) % 2;

			for (auto child : element->GetChildren())
			{
				if (!child->IsVisible())
					continue;

				if (first)
					first = false;
				else
					size[mainAxis] += m_Padding;

				glm::vec2 ps = child->GetPreferredSize();
				glm::vec2 fs = child->GetFixedSize();
				glm::vec2 fp = child->GetFixedPosition();

				glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);

				size[mainAxis] += targetSize[mainAxis];
				size[secondaryAxis] = std::max(size[secondaryAxis], targetSize[secondaryAxis] + 2 * m_Margin);
			}

			return size;
		}

		void BoxLayout::ApplyLayout(UIElementInterface* element) const
		{
			glm::vec2 fsElement = element->GetFixedSize();
			glm::vec2 weight = element->GetWeight();

			glm::vec2 containerSize(
				fsElement.x ? fsElement.x : element->GetSize().x,
				fsElement.y ? fsElement.y : element->GetSize().y
			);

			if (element->GetParent() == nullptr)
			{
				auto window = RenderCommand::GetWindow();
				containerSize.x = weight.x ? weight.x * window->GetWidth() : containerSize.x;
				containerSize.y = weight.y ? weight.y * window->GetHeight() : containerSize.y;
			}
			else
			{
				auto parentSize = element->GetParent()->GetSize();
				containerSize.x = weight.x ? weight.x * parentSize.x : containerSize.x;
				containerSize.y = weight.y ? weight.y * parentSize.y : containerSize.y;
			}
			
			if (containerSize.x < 0.f) containerSize.x = 0.f;
			if (containerSize.y < 0.f) containerSize.y = 0.f;


			int mainAxis = (int)m_Orientation;
			int secondaryAxis = (mainAxis + 1) % 2;
			int position = m_Margin;

			bool first = true;
			for (auto child : element->GetChildren())
			{
				if (!child->IsVisible())
					continue;

				glm::vec2 fp = child->GetFixedPosition();

				if (first)
					first = false;
				else if (fp == glm::vec2(0.f))
					position += m_Padding;

				glm::vec2 pos(0, 0);

				glm::vec2 ps = child->GetPreferredSize();
				glm::vec2 fs = child->GetFixedSize();
				glm::vec2 weight = child->GetWeight();

				glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);
				if (weight != glm::vec2(0.f)) targetSize = containerSize * weight;

				pos[mainAxis] = position;

				switch (m_Alignment)
				{
				case Kinematics::Alignment::Minimum:
					pos[secondaryAxis] += m_Margin;
					break;
				case Kinematics::Alignment::Middle:
					pos[secondaryAxis] += (containerSize[secondaryAxis] - targetSize[secondaryAxis]) / 2;
					break;
				case Kinematics::Alignment::Maximum:
					pos[secondaryAxis] += containerSize[secondaryAxis] - targetSize[secondaryAxis] - m_Margin * 2;
					break;
				case Kinematics::Alignment::Fill:
					pos[secondaryAxis] += m_Margin;
					targetSize[secondaryAxis] = fs[secondaryAxis] ? fs[secondaryAxis] : (containerSize[secondaryAxis] - m_Margin * 2);
					break;
				default:
					break;
				}

				
				if (fp == glm::vec2(0.f))
				{

					position += targetSize[mainAxis];
					child->SetPosition(pos);
				}
				else
				{
					child->SetPosition(fp);
				}
				child->SetSize(targetSize);
				child->ApplyLayout();

				
			}

		}

		glm::vec2 FlexLayout::GetPreferredSize(const UIElementInterface* element) const
		{
			glm::vec2 size(0.f);

			int mainAxis = (int)m_Orientation;
			int secondaryAxis = (mainAxis + 1) % 2;

			for (auto child : element->GetChildren())
			{
				if (!child->IsVisible())
					continue;

				glm::vec2 ps = child->GetPreferredSize();
				glm::vec2 fs = child->GetFixedSize();

				glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);

				size[mainAxis] += targetSize[mainAxis];
				size[secondaryAxis] = std::max(size[secondaryAxis], targetSize[secondaryAxis]);
			}

			return size;
		}

		void FlexLayout::ApplyLayout(UIElementInterface* element) const
		{
			glm::vec2 fsElement = element->GetFixedSize();
			glm::vec2 weight = element->GetWeight();

			glm::vec2 containerSize(
				fsElement.x ? fsElement.x : element->GetSize().x,
				fsElement.y ? fsElement.y : element->GetSize().y
			);

			if (element->GetParent() == nullptr)
			{
				auto window = RenderCommand::GetWindow();
				containerSize.x = weight.x ? weight.x * window->GetWidth() : containerSize.x;
				containerSize.y = weight.y ? weight.y * window->GetHeight() : containerSize.y;
			}
			else
			{
				auto parentSize = element->GetParent()->GetSize();
				containerSize.x = weight.x ? weight.x * parentSize.x : containerSize.x;
				containerSize.y = weight.y ? weight.y * parentSize.y : containerSize.y;
			}


			int mainAxis = (int)m_Orientation;
			int secondaryAxis = (mainAxis + 1) % 2;
			int position = 0;

			glm::vec2 childrenSize(0.f);

			for (auto child : element->GetChildren())
			{
				if (!child->IsVisible())
					continue;

				glm::vec2 ps = child->GetPreferredSize();
				glm::vec2 fs = child->GetFixedSize();

				glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);

				childrenSize[mainAxis] += targetSize[mainAxis];
				childrenSize[secondaryAxis] = std::max(childrenSize[secondaryAxis], targetSize[secondaryAxis]);
			}

			int freeSpace = containerSize[mainAxis] - childrenSize[mainAxis];
			if (freeSpace < 0) freeSpace = 0;

			switch (m_Justify)
			{
			case Kinematics::Justify::FlexStart:
				position = 0;
				break;
			case Kinematics::Justify::FlexEnd:
				position = freeSpace;
				break;
			case Kinematics::Justify::Center:
				position = freeSpace / 2;
				break;
			case Kinematics::Justify::SpaceBetween:
				position = 0;
				break;
			case Kinematics::Justify::SpaceAround:
				if (element->GetChildren().size() > 0)
				{
					position = freeSpace / (2 * element->GetChildren().size());
				}
				break;
			default:
				break;
			}

			for (auto child : element->GetChildren())
			{
				if (!child->IsVisible())
					continue;

				glm::vec2 pos(0, 0);

				glm::vec2 fp = child->GetFixedPosition();
				glm::vec2 ps = child->GetPreferredSize();
				glm::vec2 fs = child->GetFixedSize();

				glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);

				pos[mainAxis] = position;

				if (fp == glm::vec2(0.f))
				{

					switch (m_Justify)
					{
					case Kinematics::Justify::FlexStart:
						position += targetSize[mainAxis];
						break;
					case Kinematics::Justify::FlexEnd:
						position += targetSize[mainAxis];
						break;
					case Kinematics::Justify::Center:
						position += targetSize[mainAxis];
						break;
					case Kinematics::Justify::SpaceBetween:
						if (element->GetChildren().size() > 1)
							position += targetSize[mainAxis] + freeSpace / (element->GetChildren().size() - 1);
						break;
					case Kinematics::Justify::SpaceAround:
						if (element->GetChildren().size() > 0)
							position += targetSize[mainAxis] + freeSpace / (element->GetChildren().size());
						break;
					default:
						break;
					}

					child->SetPosition(pos);
				}
				else
				{
					child->SetPosition(fp);
				}
				child->SetSize(targetSize);
				child->ApplyLayout();
			}

		}
	}
}

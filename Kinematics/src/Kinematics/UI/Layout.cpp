#include "mtepch.h"
#include "Layout.h"

#include "UIElementInterface.h"

namespace Kinematics
{
	glm::vec2 BoxLayout::GetPreferredSize(UIElementInterface* element) const
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
		

		int mainAxis = (int)m_Orientation;
		int secondaryAxis = (mainAxis + 1) % 2;
		int position = m_Margin;

		bool first = true;
		for (auto child : element->GetChildren())
		{
			if (!child->IsVisible())
				continue;

			if (first)
				first = false;
			else
				position += m_Padding;
			
			glm::vec2 pos(0, 0);

			glm::vec2 ps = child->GetPreferredSize();
			glm::vec2 fs = child->GetFixedSize();

			glm::vec2 targetSize(fs.x ? fs.x : ps.x, fs.y ? fs.y : ps.y);

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

			child->SetPosition(pos);
			child->SetSize(targetSize);
			child->ApplyLayout();
			position += targetSize[mainAxis];
		}

	}
}

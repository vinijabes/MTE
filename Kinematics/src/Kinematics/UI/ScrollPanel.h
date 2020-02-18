#pragma once
#pragma once

#include "Panel.h"

namespace Kinematics {
	namespace UI {
		class ScrollPanel : public Panel
		{
		private:
			class ScrollContainer : public Panel
			{
			public:
				void Draw(Camera& camera, glm::vec2 pos, glm::vec2 scroll);
			};

			class ScrollBar : public Panel
			{
			public:
				ScrollBar(Orientation orientation);
				virtual void Draw(Camera& camera, glm::vec2 pos) override;
				virtual void Update(Timestep ts) override;

				inline void SetPercentage(float perc)
				{
					m_Percentage = perc;
					UpdateScrollPosition();
				}
				inline uint8_t GetPercentage() const { return m_Percentage; }

			protected:
				void UpdateScrollPosition()
				{
					auto pos = (GetSize() - m_Scroll->GetSize());
					auto mainAxis = (int)m_Orientation;

					pos[mainAxis] = pos[mainAxis] * m_Percentage;
					m_Scroll->SetPosition(pos);
				}

				void ResizeScroll()
				{
					auto overflow = m_Parent->GetPreferredSize() - m_Parent->GetSize();
					auto mainAxis = (int)m_Orientation;

					auto size = m_Size;
					size[mainAxis] = std::floorf(size[mainAxis] * std::clamp(2 * size[mainAxis] / std::abs(overflow[mainAxis] + 2 * size[mainAxis]), 0.2f, 1.f));
					size[mainAxis] -= (int)size[mainAxis] % 2 - 1;
					m_Scroll->SetSize(size);
				}
			private:
				float m_Percentage;

				Ref<UIElementInterface> m_Scroll;
				Orientation m_Orientation;
			};

		public:
			ScrollPanel();
			virtual void Draw(Camera& camera, glm::vec2 pos) override;
			virtual void Update(Timestep ts) override;

			void SetScrollPosition(glm::vec2 pos);

			virtual glm::vec2 GetPreferredSize() const
			{
				if (m_Layout)
				{
					return m_Layout->GetPreferredSize(this) + glm::vec2(m_VerticalScroll->GetSize().x, m_HorizontalScroll->GetSize().y);
				}
				else
				{
					auto size = GetSize();
					return glm::vec2(std::max(m_MinSize.x, size.x + m_VerticalScroll->GetSize().x), std::max(m_MinSize.y, size.y + m_HorizontalScroll->GetSize().y));
				}
			}

			virtual void PushChild(Ref<UIElementInterface> child)
			{
				m_ContainerPanel->PushChild(child);
			}

			virtual void PushChild(Ref<UIElementInterface> child, uint32_t pos) override
			{
				m_ContainerPanel->PushChild(child, pos);
			}

			virtual void RemoveChild(Ref<UIElementInterface> child)
			{
				m_ContainerPanel->RemoveChild(child);
			}

			void SetContainerLayout(Ref<Layout> layout) { m_ContainerPanel->SetLayout(layout); }

		protected:
			glm::vec2 m_ScrollPosition;

			Ref<ScrollContainer> m_ContainerPanel;
			Ref<ScrollBar> m_VerticalScroll;
			Ref<ScrollBar> m_HorizontalScroll;
		};
	}
}
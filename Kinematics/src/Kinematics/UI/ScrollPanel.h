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
				virtual void SetFixedSize(const glm::vec2& size);

				virtual void ApplyLayout() override { UIElementInterface::ApplyLayout(); }
			};

			class ScrollBar : public Panel
			{
				friend ScrollPanel;
			public:
				ScrollBar(Orientation orientation, Ref<ScrollContainer> container);
				virtual void Draw(Camera& camera, glm::vec2 pos) override;
				virtual void Update(Timestep ts) override;

				inline void SetPercentage(float perc)
				{
					if (perc < 0.f) perc = 0.f;
					if (perc > 1.f) perc = 1.f;

					m_Percentage = perc;
					UpdateScrollPosition();
				}
				inline uint8_t GetPercentage() const { return m_Percentage; }
				inline float GetPercentagePixelValue() const 
				{ 
					auto overflow = m_ContainerPanel->GetPreferredSize() - m_ContainerPanel->GetSize();
					return overflow[(int)m_Orientation] * 0.01f; 
				}

			protected:
				virtual bool OnMouseMovedEvent(MouseMovedEvent& e);


				void UpdateScrollPosition()
				{
					auto pos = (GetSize() - m_Scroll->GetSize());
					auto mainAxis = (int)m_Orientation;

					pos[mainAxis] = pos[mainAxis] * m_Percentage;
					m_Scroll->SetPosition(pos);
				}

				void ResizeScroll()
				{
					auto overflow = m_ContainerPanel->GetPreferredSize() - m_ContainerPanel->GetSize();
					auto mainAxis = (int)m_Orientation;

					auto size = m_Size;
					size[mainAxis] = std::floorf(size[mainAxis] * std::clamp(2 * size[mainAxis] / std::abs(overflow[mainAxis] + 2 * size[mainAxis]), 0.2f, 1.f));
					size[mainAxis] -= (int)size[mainAxis] % 2 - 1;
					m_Scroll->SetSize(size);
				}
			protected:
				float m_Percentage;
				float m_LastPercentage;

				Ref<UIElementInterface> m_Scroll;
				Ref<ScrollContainer> m_ContainerPanel;
				Orientation m_Orientation;
			};

		public:
			ScrollPanel();
			virtual void Draw(Camera& camera, glm::vec2 pos) override;
			virtual void Update(Timestep ts) override;
			virtual void OnEvent(Event& e) override;

			void UpdateContainerSize();

			virtual void SetColor(const glm::vec4& color) override { m_ContainerPanel->SetColor(color); }

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
				UpdateContainerSize();
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

			glm::vec2 m_StartScrollPosition;
			glm::vec2 m_StartMousePos;

			Ref<ScrollContainer> m_ContainerPanel;
			Ref<ScrollBar> m_VerticalScroll;
			Ref<ScrollBar> m_HorizontalScroll;
		};
	}
}
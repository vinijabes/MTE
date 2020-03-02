#pragma once

#include "ScrollPanel.h"
#include "Inflator.h"

namespace Kinematics {
	namespace UI {

		template <class Element>
		class Select : public Panel
		{
		public:
			Select();

			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;

			virtual void AddOption(const Ref<Element>& key);
			virtual void RemoveOption(const Ref<Element>& key);

			virtual Ref<UIElementInterface> InflateItem(Select* select, int index, const Ref<Element>& element);

			inline Element GetSelected() const { return m_Selected; }
			inline void SetSelected(const Ref<Element>& element)
			{
				if (ContainsElement(element))
				{
					m_Selected = element;
				}
			}

			int ContainsElement(const Ref<Element>& element)
			{
				for (int i = 0; i < m_Itens.size(); ++i)
				{
					if (m_Itens[i].first == element) return i;
				}

				return -1;
			}

			Inflator<Select*, int, const Ref<Element>&>& GetInflator() { return m_Inflator; }

		private:
			Ref<Element> m_Selected;
			std::list<Ref<Element>> m_Itens;
			std::list<Ref<UIElementInterface>> m_RenderingItens;


			Inflator<Select*, int, const Ref<Element>&> m_Inflator;

			Ref<Panel>		 m_SeletionHeader;
			Ref<ScrollPanel> m_SelectionDrop;
		};

		template<class Element>
		inline Select<Element>::Select()
			: m_Selected(nullptr)
		{
			m_SelectionDrop = CreateRef<ScrollPanel>();

			PushChild(m_SelectionDrop);
		}

		template<class Element>
		inline void Select<Element>::Draw(Camera& camera, glm::vec2 pos)
		{
			DrawChildren(camera, pos);
		}

		template<class Element>
		inline void Select<Element>::AddOption(const Ref<Element>& key)
		{
			auto item = InflateItem(this, m_Itens.size(), key);
			if (item)
			{
				m_Itens.push_back(key);
				m_RenderingItens.push_back(item);

				m_SelectionDrop->PushChild(item);
			}
		}

		template<class Element>
		inline void Select<Element>::RemoveOption(const Ref<Element> & key)
		{}

		template<class Element>
		inline Ref<UIElementInterface> Select<Element>::InflateItem(Select* select, int index, const Ref<Element>& element)
		{
			return m_Inflator.Inflate(select, index, element);
		}
	}
}
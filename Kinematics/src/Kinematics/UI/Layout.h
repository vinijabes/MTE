#pragma once

#include "Kinematics/Core/Core.h"
#include <glm/glm.hpp>


namespace Kinematics
{

	class UIElementInterface;

	enum class Alignment : uint8_t {
		Minimum = 0,
		Middle,
		Maximum,
		Fill
	};

	enum class Orientation {
		Horizontal = 0,
		Vertical
	};

	class Layout : public std::enable_shared_from_this<Layout>
	{
	public:
		Ref<Layout> GetPtr()
		{
			return shared_from_this();
		}

		virtual glm::vec2 GetPreferredSize(UIElementInterface* element) const = 0;
		virtual void ApplyLayout(UIElementInterface* element) const = 0;
	};

	class BoxLayout : public Layout
	{
	public:
		BoxLayout()
			: m_Orientation(Orientation::Horizontal),
			m_Alignment(Alignment::Middle),
			m_Margin(8),
			m_Padding(0)
		{

		}

		Orientation GetOrientation() const { return m_Orientation; }
		void SetOrientation(Orientation orientation) { m_Orientation = orientation; }

		Alignment GetAlignment() const { return m_Alignment; }
		void SetAlignment(Alignment alignment) { m_Alignment = alignment; }

		uint32_t GetMargin() const { return m_Margin; }
		void SetMargin(uint32_t margin) { m_Margin = margin; }

		uint32_t GetPadding() const { return m_Padding; }
		void SetPadding(uint32_t padding) { m_Padding = padding; }

		virtual glm::vec2 GetPreferredSize(UIElementInterface* element) const override;
		virtual void ApplyLayout(UIElementInterface* element) const override;
	private:
		Orientation m_Orientation;
		Alignment m_Alignment;

		uint32_t m_Margin;
		uint32_t m_Padding;		
	};
}
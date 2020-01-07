#pragma once

#include "Object.h"

namespace Game {
	
	class Item : virtual public Object
	{
	public:
		uint32_t GetID() const { return m_ItemID; }

	private:
		uint32_t m_ItemID;
	};
}
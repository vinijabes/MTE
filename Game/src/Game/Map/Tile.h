#pragma once

#include "Position.h"

#include <list>

namespace Game {
	class Tile
	{
		//TODO Return/Add items on tile
		//TODO Return/Add creatures on tile

		const Position& GetPosition() { return m_Position; }

	private:
		Position m_Position;
	};
}
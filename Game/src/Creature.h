#pragma once

namespace Game
{
	struct Creature {
		Creature() {};
		Creature(uint32_t id, int16_t x, int16_t y) : id(id), x(x), y(y) {}

		uint32_t id;
		int16_t x;
		int16_t y;
	};
}
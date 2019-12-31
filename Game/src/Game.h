#pragma once

#include "Messages/GameMessages.h"
#include "Creature.h"
#include <vector>
#include <memory>

namespace Game {	
	class Game {
	public:
		void PushCreature(const Creature& c) { m_Creatures.push_back(std::make_shared<Creature>(c)); }
		auto GetCreatures() { return m_Creatures; }

		std::shared_ptr<Creature> GetCreature(uint32_t id)
		{
			for (auto c : m_Creatures) if (c->id == id) return c;
			return nullptr;
		}

	private:
		std::vector<std::shared_ptr<Creature>> m_Creatures;
	};
}
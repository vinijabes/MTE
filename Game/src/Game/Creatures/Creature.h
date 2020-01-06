#pragma once

#include "Game/Map/Position.h"
#include <Kinematics.h>

namespace Game {

	enum CreatureEvent : uint16_t
	{
		CREATURE_MOVE
	};

	class Creature
	{
	public:
		Creature() : m_ID(-1) {}
		Creature(uint32_t id, Position position) : m_ID(id), m_Position(position) {}

		void Move(const Position& pos)
		{
			SetPosition(m_Position + pos);
		}

		void SetPosition(const Position& newPos) 
		{ 
			m_Position = newPos; 
			OnCreatureMove();
		}
		Position GetPosition() const { return m_Position; }

		uint32_t GetID() const { return m_ID; }

		/*SCRIPTING METHODS*/
		virtual void OnPlacedCreature() {};
		virtual void OnCreatureMove() { m_OnMoveCallback(m_ID); };

		void AddCallback(const CreatureEvent& event, Kinematics::Ref<Kinematics::Callable> cb) {
			m_OnMoveCallback = m_OnMoveCallback + std::static_pointer_cast<Kinematics::CallbackInterface<void, int>>(cb);
		};

	private:
		uint32_t m_ID;
		Position m_Position;

		Kinematics::Callback<void, int> m_OnMoveCallback;
	};
}
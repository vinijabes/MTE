#pragma once

#include "Game/Map/Position.h"
#include "Game/Objects/Object.h"

#include <Kinematics.h>

namespace Game {

	enum class CreatureEvent : uint16_t
	{
		CREATURE_MOVE,
		CREATURE_PLACED
	};

	typedef Kinematics::Callback<void, int, Position, Position> OnMoveCallback;
	typedef Kinematics::LuaCallback<void, int, Position, Position> OnMoveLuaCallback;
	typedef Kinematics::ScriptCallable<void, int, Position, Position> OnMoveCallable;
	typedef Kinematics::CallbackInterface<void, int, Position, Position> OnMoveCallbackInterface;

	class Creature : public virtual Object
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
			OnCreatureMoved(m_ID, m_Position, newPos);
			m_Position = newPos;
		}
		Position GetPosition() const { return m_Position; }
		uint32_t GetID() const { return m_ID; }

		/*SCRIPTING METHODS*/
		virtual void OnPlacedCreature() {};
		virtual void OnCreatureMoved(const uint32_t& id, const Position& oldPos, const Position& newPos)
		{
			m_OnMoveCallback(m_ID, oldPos, newPos);
		}

		void AddCallback(const CreatureEvent& event, Kinematics::Ref<Kinematics::Callable> cb);

	private:
		uint32_t m_ID;
		Position m_Position;

		OnMoveCallback m_OnMoveCallback;
	};
}
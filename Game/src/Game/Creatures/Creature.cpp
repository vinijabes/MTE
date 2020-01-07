#include "Creature.h"

namespace Game {

	void Creature::AddCallback(const CreatureEvent& event, Kinematics::Ref<Kinematics::Callable> cb)
	{
		switch (event)
		{
			case CreatureEvent::CREATURE_MOVE:
				m_OnMoveCallback = m_OnMoveCallback + std::static_pointer_cast<OnMoveCallbackInterface>(cb);
				break;
			default:
				break;
		}
	}

}
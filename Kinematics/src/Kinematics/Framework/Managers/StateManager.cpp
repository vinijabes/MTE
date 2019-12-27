#include "mtepch.h"
#include "StateManager.h"

#define EVENT_IMEDIAT 1

namespace Kinematics {
	StateManager* StateManager::m_Instance = NULL;

	void StateManager::Emit(Event* message)
	{
#ifdef EVENT_IMEDIAT
		for (auto observer : m_Observers[message->GetEventType()])
		{
			observer(*message);
		}
		delete message;
#else
		m_Messages.push(message);
#endif
	}

	void StateManager::On(EventType type, EventFn observer)
	{
		m_Observers[type].push_back(observer);
	}

	void StateManager::NotifyAll()
	{
		while (!m_Messages.empty())
		{
			Event* e = m_Messages.front();
			for (auto observer : m_Observers[e->GetEventType()])
			{
				observer(*e);
			}

			m_Messages.pop();
			delete e;
		}
	}
}
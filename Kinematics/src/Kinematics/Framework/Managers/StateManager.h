#pragma once

#include "Kinematics/Core/Hash.h"
#include "Kinematics/Core/Core.h"
#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <unordered_map>
#include <vector>
#include <queue>

namespace Kinematics {
	typedef uint32_t MessageType;

	class StateManager
	{
	public:
		StateManager(StateManager const&) = delete;
		void operator=(StateManager const&) = delete;

		static StateManager* GetInstance()
		{
			if (!m_Instance)
			{
				m_Instance = new StateManager();
			}

			return m_Instance;
		}

		void Emit(Event* message);
		void On(EventType type, EventFn callback);

		void NotifyAll();

	private:
		StateManager() {}
	private:
		static StateManager* m_Instance;
		std::unordered_map<EventType, std::vector<EventFn>> m_Observers;
		std::queue<Event*> m_Messages;
	};
}
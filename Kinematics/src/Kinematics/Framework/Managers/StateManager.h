#pragma once

#include "Kinematics/Core/Hash.h"
#include "Kinematics/Core/Core.h"
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <unordered_map>
#include <vector>

namespace Kinematics {
	typedef uint32_t MessageType;

	class StateManager
	{
	public:
		class BaseMessage
		{
		public:
			BaseMessage(MessageType type) 
			{
				m_Type = type;
			}

		private:
			MessageType m_Type;
		};
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

		void SendMessage(BaseMessage message);
		void SendMessage(const BaseMessage& message);

		void Notify();

	private:
		StateManager() {}
	private:
		static StateManager* m_Instance;
		std::unordered_map<MessageType, std::vector<Ref<SubSystemInterface>>> m_Observers;
	};
}
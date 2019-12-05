#pragma once
#include <functional>
#include <queue>
#include <vector>

#include "Message.h"

namespace Kinematics {
	class MessageBus {
	public:
		MessageBus() {};
		~MessageBus() {};

		void AddReceiver(std::function<void(Message)> MessageReceiver)
		{
			m_Receivers.push_back(MessageReceiver);
		}

		void SendMessage(Message message) 
		{
			m_Messages.push(message);
		}

		void Notify() 
		{
			while(!m_Messages.empty())
			{
				for (auto it = m_Receivers.begin(); it != m_Receivers.end(); it++) 
				{
					(*it)(m_Messages.front());
				}

				m_Messages.pop();
			}
		}

	private:
		std::vector<std::function< void(Message)>> m_Receivers;
		std::queue<Message> m_Messages;
	};
}
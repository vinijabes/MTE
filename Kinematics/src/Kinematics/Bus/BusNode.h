#pragma once
#include <iostream>
#include <functional>

#include "Message.h"
#include "MessageBus.h"

namespace MTE {

	class BusNode
	{
	public:
		BusNode(MessageBus* messageBus)
		{
			this->m_MessageBus = messageBus;
			this->m_MessageBus->AddReceiver(this->getNotifyFunc());
		}

		virtual void update() {};
	protected:
		MessageBus* m_MessageBus;

		std::function<void(Message)> getNotifyFunc()
		{
			auto messageListener = [=](Message message) -> void {
				this->OnNotify(message);
			};
			return messageListener;
		}

		void Send(Message message)
		{
			m_MessageBus->SendMessage(message);
		}

		virtual void OnNotify(Message message)
		{
			// Do something here. Your choice. You could do something like this.
			// std::cout << "Siopao! Siopao! Siopao! (Someone forgot to implement onNotify().)" << std::endl;
		}
	};
}
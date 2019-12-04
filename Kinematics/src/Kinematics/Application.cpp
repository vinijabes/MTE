#include "mtepch.h"
#include "Application.h"
#include "Semaphore.h"
#include "./Bus/MessageBus.h"
#include "./Bus/BusNode.h"
#include <atomic>
#include <thread>
#include <Windows.h>
#include <conio.h>

namespace MTE {

	Semaphore iSemaphore(1);
	Semaphore oSemaphore(1);

	class InputSystem : public BusNode {
	public:
		InputSystem(MessageBus* messageBus)
			: BusNode(messageBus) {}

		void Run()
		{
			m_Running = true;
			while (m_Running.load())
			{
				iSemaphore.Down();

				if (GetKeyState(65) & 0x8000) {
					this->m_MessageBus->SendMessage(Message("A"));
				}

				if (GetKeyState(VK_UP) & 0x8000) {
					this->m_MessageBus->SendMessage(Message("SETA PRA CIMA"));
				}

				if (GetKeyState(VK_ESCAPE) & 0x8000)
				{
					this->m_MessageBus->SendMessage(Message("Stop"));
				}
			}
		}

		void Stop()
		{
			m_Running = false;
		}

		void OnNotify(Message message) override
		{
			if (message.getEvent() == "Stop") this->Stop();
			std::cout << message.getEvent() << std::endl;
		}

	private:
		std::atomic<bool> m_Running;
	};

	class OutputSystem : public BusNode {
	public:
		OutputSystem(MessageBus* messageBus)
			: BusNode(messageBus) {}

		void Run()
		{
			m_Running = true;
			while (m_Running.load())
			{
				oSemaphore.Down();
			}
		}

		void Stop()
		{
			m_Running = false;
		}

		void OnNotify(Message message) override
		{
			if (message.getEvent() == "Stop") this->Stop();
			std::cout << message.getEvent() << std::endl;
		}

	private:
		std::atomic<bool> m_Running;
	};

	void foo(int Z)
	{
		//s.Down();
		for (int i = 0; i < Z; i++)
		{
			std::cout << i << std::endl;
		}
	}

	void Application::Run()
	{
		m_Running = true;

		MessageBus* MB = new MessageBus();
		InputSystem IS(MB);
		OutputSystem OS(MB);

		if (dynamic_cast<MessageBus*>(&IS)) std::cout << "Salve!" << std::endl;
		else std::cout << "Nao salve!" << std::endl;

		MB->AddReceiver([=](Message message) -> void {
			if (message.getEvent() == "Stop") {
				this->Stop();
			}
			});

		MB->SendMessage(Message("10"));
		MB->Notify();

		std::thread ISth(&InputSystem::Run, &IS);
		std::thread OSth(&OutputSystem::Run, &OS);

		while (m_Running) {
			if (iSemaphore.GetValue() == -1 && oSemaphore.GetValue() == -1)
			{
				iSemaphore.Up();
				oSemaphore.Up();
				MB->Notify();
			}
		}

		iSemaphore.Up();
		oSemaphore.Up();


		ISth.join();
		OSth.join();
	}
}
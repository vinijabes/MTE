// Sandbox.cpp : Este arquivo cont�m a fun��o 'main'. A execu��o do programa come�a e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

class Client : public Kinematics::Application
{
public:
	Client()
		: Application()
	{
	}

	~Client()
	{

	}

	void Run() override
	{
		srand(time(NULL));
		Kinematics::Framework framework;
		framework.Initialize();

		framework.AddSubSystem("WindowSubSystem");
		framework.AddSubSystem("NetworkSubSystem");
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Connect("172.18.44.49", DEFAULT_PORT);
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("disconnection", [](Kinematics::NetworkMessage& message) {
			KINEMATICS_TRACE("Client Disconnected!");
		});

		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("connection", [](Kinematics::NetworkMessage& message) {
			KINEMATICS_TRACE("Connected!");
		});

		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->Emit("v", Kinematics::v());

		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowClose, [=](Kinematics::Event& e) {
			this->Stop();
			return false;
			});

		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowResize, [=](Kinematics::Event& e) {
			Kinematics::WindowResizeEvent* we = (Kinematics::WindowResizeEvent*) & e;
			return false;
			});

		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseMoved, [=](Kinematics::Event& e) {
			Kinematics::MouseMovedEvent* me = (Kinematics::MouseMovedEvent*) & e;
			return false;
			});

		while (this->m_Running)
		{
			framework.Update();
		}

		framework.Shutdown();
	}

	void Stop() override
	{
		m_Running = false;
	}

private:
	std::atomic<bool> m_Running = true;
};

Client::Application* Kinematics::CreateApplication()
{
	return new Client();
}
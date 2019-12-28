// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
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

		framework.AddSubSystem("NetworkSubSystem");
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Listen(DEFAULT_PORT);
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetServer()->On("connection", [](Kinematics::NetworkMessage& message) {
			KINEMATICS_TRACE("Client connected!");
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
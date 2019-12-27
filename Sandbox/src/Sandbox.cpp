// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

class Sandbox : public Kinematics::Application
{
public:
	Sandbox()
		: Application()
	{
	}

	~Sandbox()
	{

	}

	void Run() override
	{
		srand(time(NULL));
		Kinematics::Framework framework;
		framework.Initialize();

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

Sandbox::Application* Kinematics::CreateApplication()
{
	return new Sandbox();
}
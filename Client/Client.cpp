// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <Game.h>

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
		Game::Game game;
		framework.Initialize();

		framework.AddSubSystem("WindowSubSystem");
		framework.AddSubSystem("NetworkSubSystem");
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Connect("127.0.0.1", DEFAULT_PORT);
		
		auto ref = &framework;
		
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("disconnection", [](Kinematics::NetworkMessage& message) {
			KINEMATICS_TRACE("Client Disconnected!");
		});

		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("connection", [](Kinematics::NetworkMessage& message) {
			KINEMATICS_TRACE("Connected!");
		});		

		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("data", [=, &game](Kinematics::NetworkMessage& message) {
			Game::DataMessage* dm = (Game::DataMessage*) & message;

			ref->GetSubSystem<Kinematics::WindowSubSystemInterface>()->ClearList();

			for (auto obj : dm->GetObjects())
			{
				if(!game.GetCreature(obj.id))
					game.PushCreature(obj);
				ref->GetSubSystem<Kinematics::WindowSubSystemInterface>()->Submit(obj.x, obj.y);
			}
		});

		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("creatureUpdate", [=, &game](Kinematics::NetworkMessage& message) {
			Game::CreatureUpdateMessage* dm = (Game::CreatureUpdateMessage*) & message;
			auto creature = game.GetCreature(dm->GetID());
			creature->x = dm->GetX();
			creature->y = dm->GetY();

			ref->GetSubSystem<Kinematics::WindowSubSystemInterface>()->ClearList();
			for (auto obj : game.GetCreatures())
			{
				ref->GetSubSystem<Kinematics::WindowSubSystemInterface>()->Submit(obj->x, obj->y);
			}
		});


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

		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::KeyPressed, [=](Kinematics::Event& e) {
			Kinematics::KeyPressedEvent* ke = (Kinematics::KeyPressedEvent*) & e;

			auto message = Game::InputMessage(ke->GetKeyCode());
			ref->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->Emit("input", message);
			KINEMATICS_TRACE("KeyPress: {}", ke->GetKeyCode());
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
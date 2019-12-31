// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <Game.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

volatile int x = 0;
volatile int y = 0;

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

		framework.AddSubSystem("NetworkSubSystem");
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Listen(DEFAULT_PORT);
		framework.GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetServer()->On("connection", [&](Kinematics::NetworkMessage& message) {
			Kinematics::ConnectionMessage* cm = (Kinematics::ConnectionMessage*) & message;
			auto socket = cm->GetSocket();

			game.PushCreature(Game::Creature(socket->GetID(), 0, 0));

			auto m_X = new int(0);
			auto m_Y = new int(0);

			socket->On("input", [=, &framework, &game](Kinematics::NetworkMessage& message) {
				Game::InputMessage* im = (Game::InputMessage*) & message;
				KINEMATICS_TRACE("Key pressed: {}", im->GetKey());

				auto m_Creature = game.GetCreature(socket->GetID());

				if (im->GetKey() == KINEMATICS_KEY_LEFT)
				{
					;
					auto dm = Game::CreatureUpdateMessage(m_Creature->id, --m_Creature->x, m_Creature->y);
					socket->GetServer()->Emit("creatureUpdate", dm);
				}

				if (im->GetKey() == KINEMATICS_KEY_RIGHT)
				{
					auto dm = Game::CreatureUpdateMessage(m_Creature->id, ++m_Creature->x, m_Creature->y);
					socket->GetServer()->Emit("creatureUpdate", dm);
				}

				if (im->GetKey() == KINEMATICS_KEY_UP)
				{
					auto dm = Game::CreatureUpdateMessage(m_Creature->id, m_Creature->x, ++m_Creature->y);
					socket->GetServer()->Emit("creatureUpdate", dm);
				}

				if (im->GetKey() == KINEMATICS_KEY_DOWN)
				{
					auto dm = Game::CreatureUpdateMessage(m_Creature->id, m_Creature->x, --m_Creature->y);
					socket->GetServer()->Emit("creatureUpdate", dm);
				}
			});

			std::vector<Game::Creature> creatures;
			for (auto c : game.GetCreatures())
			{
				creatures.push_back(*c);
			}

			auto dm = Game::DataMessage(creatures);
			socket->GetServer()->Emit("data", dm);
			socket->On("disconnection", [=](Kinematics::NetworkMessage& message){

			});

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
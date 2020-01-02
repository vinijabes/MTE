// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <Game.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

class GameLayer : public Kinematics::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Kinematics::Timestep ts) override;
	void OnEvent(Kinematics::Event& e) override;

private:	
	Game::Game m_Game;	
};

GameLayer::GameLayer()
	: Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{	

	Kinematics::Application::Get().GetFramework()->AddSubSystem("NetworkSubSystem");
	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Listen(DEFAULT_PORT);
	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetServer()->On("connection", [&](Kinematics::NetworkMessage& message) {
		Kinematics::ConnectionMessage* cm = (Kinematics::ConnectionMessage*) & message;
		auto socket = cm->GetSocket();

		m_Game.PushCreature(Game::Creature(socket->GetID(), 0, 0));

		auto m_X = new int(0);
		auto m_Y = new int(0);

		socket->On("input", [=](Kinematics::NetworkMessage& message) {
			Game::InputMessage* im = (Game::InputMessage*) & message;
			KINEMATICS_TRACE("Key pressed: {}", im->GetKey());

			auto m_Creature = m_Game.GetCreature(socket->GetID());

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

		socket->On("disconnection", [=](Kinematics::NetworkMessage& message) {
			Kinematics::DisconnectionMessage* dm = (Kinematics::DisconnectionMessage*) & message;

			uint32_t id = dm->GetID();
			m_Game.RemoveCreature(id);

			auto pdm = Game::PlayerDisconnectMessage(id);
			socket->Broadcast("playerDisconnection", pdm);
		});

		std::vector<Game::Creature> creatures;
		for (auto c : m_Game.GetCreatures())
		{
			creatures.push_back(*c);
		}

		auto dm = Game::DataMessage(creatures);
		socket->GetServer()->Emit("data", dm);

		auto gcm = Game::GameConnectedMessage(socket->GetID());
		socket->Emit("gameConnected", gcm);
	});
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Kinematics::Timestep ts)
{
}

void GameLayer::OnEvent(Kinematics::Event& e)
{
}


class Client : public Kinematics::Application
{
public:
	Client()
		: Application()
	{
		PushLayer(new GameLayer());
	}

	~Client()
	{

	}
private:
	std::atomic<bool> m_Running = true;
};

Client::Application* Kinematics::CreateApplication()
{
	return new Client();
}
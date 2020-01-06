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
	Kinematics::Ref<Kinematics::Script> mainScript = Kinematics::Script::Create("Scripts/main.lua");
	mainScript->Run();

	Kinematics::Resources::Add("MainScript", mainScript);

	Kinematics::Application::Get().GetFramework()->AddSubSystem("NetworkSubSystem");
	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Listen(DEFAULT_PORT);
	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetServer()->On("connection", [&](Kinematics::NetworkMessage& message) {
		Kinematics::ConnectionMessage* cm = (Kinematics::ConnectionMessage*) & message;
		auto socket = cm->GetSocket();

		auto OnMoveLua = Kinematics::ScriptCallable<void, int>(Kinematics::Resources::Get<Kinematics::Script>("MainScript"), "OnMove");
		auto OnMoveLuaCallback = Kinematics::CreateRef<Kinematics::LuaCallback<void, int>>(OnMoveLua);

		auto creature = Game::Creature(socket->GetID(), Game::Position(0, 0, 0));
		creature.AddCallback(Game::CreatureEvent::CREATURE_MOVE, OnMoveLuaCallback);
		m_Game.PushCreature(creature);

		auto m_X = new int(0);
		auto m_Y = new int(0);

		socket->On("input", [=](Kinematics::NetworkMessage& message) {
			Game::InputMessage* im = (Game::InputMessage*) & message;
			KINEMATICS_TRACE("Key pressed: {}", im->GetKey());

			auto m_Creature = m_Game.GetCreature(socket->GetID());

			if (im->GetKey() == KINEMATICS_KEY_LEFT)
			{
				m_Creature->Move(Game::Position(-1, 0, 0));
				auto dm = Game::CreatureUpdateMessage(m_Creature->GetID(), m_Creature->GetPosition().GetX(), m_Creature->GetPosition().GetY());
				socket->GetServer()->Emit("creatureUpdate", dm);
			}

			if (im->GetKey() == KINEMATICS_KEY_RIGHT)
			{
				m_Creature->Move(Game::Position(1, 0, 0));
				auto dm = Game::CreatureUpdateMessage(m_Creature->GetID(), m_Creature->GetPosition().GetX(), m_Creature->GetPosition().GetY());
				socket->GetServer()->Emit("creatureUpdate", dm);
			}

			if (im->GetKey() == KINEMATICS_KEY_UP)
			{
				m_Creature->Move(Game::Position(0, 1, 0));
				auto dm = Game::CreatureUpdateMessage(m_Creature->GetID(), m_Creature->GetPosition().GetX(), m_Creature->GetPosition().GetY());
				socket->GetServer()->Emit("creatureUpdate", dm);
			}

			if (im->GetKey() == KINEMATICS_KEY_DOWN)
			{
				m_Creature->Move(Game::Position(0, -1, 0));
				auto dm = Game::CreatureUpdateMessage(m_Creature->GetID(), m_Creature->GetPosition().GetX(), m_Creature->GetPosition().GetY());
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
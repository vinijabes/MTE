// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <Game.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

#include <tinyxml2/tinyxml2.h>


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
	Kinematics::OrthographicCameraController m_CameraController;
	Kinematics::PerspectiveCamera m_Camera;
	Kinematics::Ref<Kinematics::WindowSubSystemInterface> m_Window;
	Game::Game m_Game;
	std::shared_ptr<Game::Creature> m_Player;

	Kinematics::Ref<Kinematics::Texture2D> m_PlayerTexture;
	Kinematics::Ref<Kinematics::Sprite> m_PlayerSprite;
	Kinematics::Ref<Kinematics::Shader> m_Shader;
};

GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(640.0f / 480.0f, true), m_Camera(60.0f, 16.0f / 9.0f, 0.001f, 3.2f)
{
}

int testef(Kinematics::ScriptState& state)
{
	KINEMATICS_TRACE("C FUNCTION");
	return 0;
}

void GameLayer::OnAttach()
{
	Kinematics::Application::Get().GetFramework()->AddSubSystem("WindowSubSystem");
	Kinematics::Application::Get().GetFramework()->AddSubSystem("NetworkSubSystem");

	m_Window = Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::WindowSubSystemInterface>();
	m_PlayerTexture = Kinematics::Texture2D::Create("assets/textures/fox-run.png", Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE, Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE);


	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("Resources.xml") != tinyxml2::XML_SUCCESS) KINEMATICS_ERROR("Error loading resources!");
	else
	{
		auto textures = doc.FirstChildElement("Resources")->FirstChildElement("Textures");
		for (auto texture = textures->FirstChildElement(); texture != NULL; texture = texture->NextSiblingElement())
		{
			Kinematics::Resources::Add(texture->Attribute("id"), Kinematics::Texture2D::Create(texture->Attribute("src"), Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE, Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE));
		}

		auto sprites = doc.FirstChildElement("Resources")->FirstChildElement("Sprites");
		for (auto sprite = sprites->FirstChildElement(); sprite != NULL; sprite = sprite->NextSiblingElement())
		{
			const char* id = sprite->Attribute("id");
			float animationTime;
			int frameCount;

			sprite->FirstChildElement("FrameCount")->QueryIntText(&frameCount);
			sprite->FirstChildElement("AnimationTime")->QueryFloatText(&animationTime);
			const char* textureId = sprite->FirstChildElement("TextureID")->GetText();

			Kinematics::Resources::Add(id, Kinematics::CreateRef<Kinematics::Sprite>(Kinematics::Resources::Get<Kinematics::Texture2D>(textureId), frameCount, animationTime));
		}
	}

	Kinematics::Resources::Add("chair", Kinematics::Model::Load("assets/models/chair.obj")->GetMeshes()[0]);
	m_Shader = Kinematics::Shader::Create("assets/shaders/Object.glsl");

	m_PlayerSprite = Kinematics::Resources::Get<Kinematics::Sprite>("player");
	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->Connect("127.0.0.1", DEFAULT_PORT);

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("disconnection", [](Kinematics::NetworkMessage& message) {
		KINEMATICS_TRACE("Client Disconnected!");
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("connection", [](Kinematics::NetworkMessage& message) {
		KINEMATICS_TRACE("Connected!");
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("gameConnected", [&](Kinematics::NetworkMessage& message) {
		Game::GameConnectedMessage* gcm = (Game::GameConnectedMessage*) & message;

		m_Player = m_Game.GetCreature(gcm->GetID());
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("data", [&](Kinematics::NetworkMessage& message) {
		Game::DataMessage* dm = (Game::DataMessage*) & message;

		for (auto obj : dm->GetObjects())
		{
			if (!m_Game.GetCreature(obj.GetID()))
				m_Game.PushCreature(obj);
		}
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("creatureUpdate", [=](Kinematics::NetworkMessage& message) {
		Game::CreatureUpdateMessage* dm = (Game::CreatureUpdateMessage*) & message;
		auto creature = m_Game.GetCreature(dm->GetID());
		creature->GetPosition().SetX(dm->GetX());
		creature->GetPosition().SetY(dm->GetY());

		auto id = Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->GetID();
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("playerDisconnection", [=](Kinematics::NetworkMessage& message) {
		Game::PlayerDisconnectMessage* pdm = (Game::PlayerDisconnectMessage*) & message;
		m_Game.RemoveCreature(pdm->GetID());
		});

	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowResize, [=](Kinematics::Event& e) {
		Kinematics::WindowResizeEvent* we = (Kinematics::WindowResizeEvent*) & e;
		});

	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseMoved, [=](Kinematics::Event& e) {
		Kinematics::MouseMovedEvent* me = (Kinematics::MouseMovedEvent*) & e;
		});

	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::KeyPressed, [=](Kinematics::Event& e) {
		Kinematics::KeyPressedEvent* ke = (Kinematics::KeyPressedEvent*) & e;

		auto message = Game::InputMessage(ke->GetKeyCode());
		Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->Emit("input", message);
		KINEMATICS_TRACE("KeyPress: {}", ke->GetKeyCode());
	});
}

void GameLayer::OnDetach()
{
}


void GameLayer::OnUpdate(Kinematics::Timestep ts)
{
	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();
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
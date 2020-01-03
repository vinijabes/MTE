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
	Kinematics::OrthographicCameraController m_CameraController;
	Kinematics::Ref<Kinematics::WindowSubSystemInterface> m_Window;
	Game::Game m_Game;
	std::shared_ptr<Game::Creature> m_Player;

	Kinematics::Ref<Kinematics::Texture2D> m_PlayerTexture;
	Kinematics::Ref<Kinematics::Sprite> m_PlayerSprite;
};

GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(640.0f / 480.0f, true)
{
}

void GameLayer::OnAttach()
{

	Kinematics::Application::Get().GetFramework()->AddSubSystem("WindowSubSystem");
	Kinematics::Application::Get().GetFramework()->AddSubSystem("NetworkSubSystem");

	m_Window = Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::WindowSubSystemInterface>();
	m_PlayerTexture = Kinematics::Texture2D::Create("assets/textures/fox-run.png", Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE, Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE);
	
	Kinematics::Resources::Add("fox", m_PlayerTexture);
	m_PlayerSprite = Kinematics::CreateRef<Kinematics::Sprite>(Kinematics::Resources::Get<Kinematics::Texture2D>("fox"), 6, 500);

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
			if (!m_Game.GetCreature(obj.id))
				m_Game.PushCreature(obj);
		}
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("creatureUpdate", [=](Kinematics::NetworkMessage& message) {
		Game::CreatureUpdateMessage* dm = (Game::CreatureUpdateMessage*) & message;
		auto creature = m_Game.GetCreature(dm->GetID());
		creature->x = dm->GetX();
		creature->y = dm->GetY();

		auto id = Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->GetID();
		});

	Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->On("playerDisconnection", [=](Kinematics::NetworkMessage& message) {
		Game::PlayerDisconnectMessage* pdm = (Game::PlayerDisconnectMessage*) & message;
		m_Game.RemoveCreature(pdm->GetID());
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
		Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::NetworkSubSystemInterface>()->GetClient()->Emit("input", message);
		KINEMATICS_TRACE("KeyPress: {}", ke->GetKeyCode());
		return false;
		});
}

void GameLayer::OnDetach()
{
}

int i = 0;
float atime = 0;

void GameLayer::OnUpdate(Kinematics::Timestep ts)
{
	float aspect = (float)m_Window->GetWidth() / m_Window->GetHeight();
	float tileSize = (aspect * 2) / (m_Window->GetWidth() / 64);

	float verticalSize = 2.0f / (m_Window->GetHeight() / 64);

	if (m_Player)
		m_CameraController.MoveTo({ m_Player->x * tileSize - tileSize / 2, m_Player->y * verticalSize , 0 });
	m_CameraController.OnUpdate(ts);
	m_PlayerSprite->OnUpdate(ts);

	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();

	Kinematics::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Kinematics::Renderer2D::BeginScene(m_CameraController.GetCamera());


	if (i > 5) i = 0;
	for (auto obj : m_Game.GetCreatures())
	{
		Kinematics::Renderer2D::DrawQuad({ obj->x * tileSize - tileSize / 2, obj->y * verticalSize }, { tileSize, verticalSize }, m_PlayerSprite);
	}

	atime = atime + ts.GetMilliseconds();
	if (atime > 200) {
		i++;
		atime = 0;
	}
	Kinematics::Renderer2D::EndScene();
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
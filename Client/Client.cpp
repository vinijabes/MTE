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

int testef(Kinematics::ScriptState& state)
{
	KINEMATICS_TRACE("C FUNCTION");
	return 0;
}

class MyObject
{
private:
	double x;
public:
	MyObject(double x) : x(x) {}
	void set(double x) { this->x = x; }
	double get() const { return this->x; }


	static int MyObjNew(Kinematics::ScriptState& state);
	static int MyObjDelete(Kinematics::ScriptState& state);

	static int MyObjSet(Kinematics::ScriptState& state);
	static int MyObjGet(Kinematics::ScriptState& state);

	static void RegisterScriptConstructor(Kinematics::Script& state)
	{
		std::vector<Kinematics::Ref<Kinematics::ScriptWrapperContainer>> methods;
		methods.push_back(Kinematics::Ref<Kinematics::ScriptWrapperContainer>(Kinematics::LuaScript::WrapPointer<MyObject::MyObjSet>("set")));
		methods.push_back(Kinematics::Ref<Kinematics::ScriptWrapperContainer>(Kinematics::LuaScript::WrapPointer<MyObject::MyObjGet>("get")));
		auto newRef = Kinematics::LuaScript::Wrap<MyObject::MyObjNew>();
		auto deleteRef = Kinematics::LuaScript::Wrap<MyObject::MyObjDelete>();

		state.CreateMetaTable("MyObject",
			newRef,
			deleteRef,
			methods);
	}
};

int MyObject::MyObjNew(Kinematics::ScriptState& state)
{
	float x = state.Get().GetParameter<float>(1);
	*state.Get().CreateUserData<MyObject>() = new MyObject(x);
	state.Get().SetMetaTable("MyObject");

	return 1;
}

int MyObject::MyObjDelete(Kinematics::ScriptState& state)
{
	delete* state.Get().GetUserData<MyObject>(1);

	return 0;
}

int MyObject::MyObjSet(Kinematics::ScriptState& state)
{
	(*state.Get().GetUserDataParameter<MyObject>(1, "MyObject"))->set(state.Get().GetParameter<float>(2));

	return 0;
}

int MyObject::MyObjGet(Kinematics::ScriptState& state)
{
	state.Get().Push<float>((*state.Get().GetUserDataParameter<MyObject>(1, "MyObject"))->get());
	return 1;
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
	float aspect = (float)m_Window->GetWidth() / m_Window->GetHeight();
	float tileSize = (aspect * 2) / (m_Window->GetWidth() / 64);

	float verticalSize = 2.0f / (m_Window->GetHeight() / 64);

	if (m_Player)
		m_CameraController.MoveTo({ m_Player->GetPosition().GetX() * tileSize, m_Player->GetPosition().GetY() * verticalSize , 0 });
	m_CameraController.OnUpdate(ts);
	m_PlayerSprite->OnUpdate(ts);

	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();

	Kinematics::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			Kinematics::Renderer2D::DrawQuad({ (i - 7) * tileSize, (j - 3) * verticalSize }, { tileSize,  verticalSize }, Kinematics::Resources::Get<Kinematics::Texture2D>("0"));
		}
	}

	Kinematics::Renderer2D::DrawQuad({ 0 * tileSize, 2 * verticalSize, 0.1f }, { tileSize,  verticalSize }, Kinematics::Resources::Get<Kinematics::Texture2D>("1"));

	for (auto obj : m_Game.GetCreatures())
	{		
		Kinematics::Renderer2D::DrawQuad({ obj->GetPosition().GetX() * tileSize, obj->GetPosition().GetY() * verticalSize, 1.0f }, { tileSize, verticalSize }, m_PlayerSprite);
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
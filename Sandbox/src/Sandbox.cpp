// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

class GameLayer : public Kinematics::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Kinematics::Timestep ts) override;
	void OnEvent(Kinematics::Event& e) override;

protected:
	Kinematics::Ref<Kinematics::Mesh> m_Mesh;
	Kinematics::Ref<Kinematics::Model> m_Model;
	Kinematics::Ref<Kinematics::Model> m_ModelPlayer;
	Kinematics::Ref<Kinematics::Shader> m_Shader;
	Kinematics::PerspectiveCamera m_Camera;
	Kinematics::OrthographicCameraController m_UICamera;
	Kinematics::Ref<Kinematics::UI::Window> m_UIWindow;

	Kinematics::Ref<Kinematics::FrameBuffer> m_FrameBuffer;
	Kinematics::Ref<Kinematics::Text> m_Text;
	Kinematics::Ref<Kinematics::Texture2D> m_Texture;
};

GameLayer::GameLayer()
	: Layer("GameLayer"), m_Camera(glm::degrees(2 * std::atan2(0.000275, 0.001f)), 8.5f / 5.5f, 0.001f, 30.0f), m_UICamera(1, false)
{
	m_UICamera.LockAspect();
}

void GameLayer::OnAttach()
{
	Kinematics::Application::Get().GetFramework()->AddSubSystem("WindowSubSystem");
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::KeyPressed, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::CharacterTyped, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseMoved, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseButtonReleased, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseButtonPressed, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseScrolled, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowResize, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));

	Kinematics::RenderCommand::SetWindow(Kinematics::Application::Get().GetFramework()->GetSubSystem<Kinematics::WindowSubSystemInterface>().get());
	m_Mesh = Kinematics::Mesh::Create();

	float vertices[] = {
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	   -0.5f,  0.5f, 0.0f
	};
	float textureCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float color[] = {
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
	};

	float offset[] = {
		0.0f,  0.0f, 0.0f,
		0.5f,  0.0f, 0.0f,
		0.0f,  0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
	   -0.5f,  0.0f, 0.0f,
	};

	m_Shader = Kinematics::Shader::Create("assets/shaders/3DTexture.glsl");
	m_Shader->Bind();
	for (int i = 0; i < 32; i++)
	{
		m_Shader->SetInt("u_Textures[" + std::to_string(i) + "]", i);
	}

	m_Model = Kinematics::Model::Load("assets/models/MultipleTextureCube.obj");
	m_ModelPlayer = Kinematics::Model::Load("assets/models/Character.obj");
	m_Mesh = m_Model->GetMeshes()[0];


	Kinematics::Resources::Add("0", Kinematics::Texture2D::Create("assets/textures/0.png", Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE, Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE));

	auto roboto = Kinematics::FontManager::GetInstance()->Load("assets/fonts/Roboto-Regular.ttf");
	Kinematics::Resources::Add("Roboto", roboto);

	auto box = Kinematics::CreateRef<Kinematics::UI::Select<int>>();
	box->GetInflator().SetInflator([](Kinematics::UI::Select<int>* select, int index, const Kinematics::Ref<int>& element) -> Kinematics::Ref<Kinematics::UI::UIElementInterface>{
		auto box = Kinematics::CreateRef<Kinematics::UI::TextBox>();
		box->SetText(std::to_string(*element));

		return box;
	});

	box->AddOption(Kinematics::CreateRef<int>(10));
	box->AddOption(Kinematics::CreateRef<int>(11));
	box->AddOption(Kinematics::CreateRef<int>(12));
	box->AddOption(Kinematics::CreateRef<int>(13));
	box->AddOption(Kinematics::CreateRef<int>(14));

	m_UIWindow = Kinematics::CreateRef<Kinematics::UI::Window>();

	m_UIWindow->SetTitle("Window Title");
	m_UIWindow->SetTheme(Kinematics::Theme::DARK_THEME);
	m_UIWindow->SetWeight({ 1.f, 1.f });
	m_UIWindow->SetFullSize(true);

	m_UIWindow->PushChild(box);
	
	m_UIWindow->ApplyLayout();
}

void GameLayer::OnDetach()
{
}

float sum = 0;

void GameLayer::OnUpdate(Kinematics::Timestep ts)
{
	//sum += ts.GetSeconds();
	//m_Shader->SetInt("u_LightCount", 1);
	//m_Shader->SetFloat3("u_ViewerPos", m_Camera.GetPosition());
	//m_Shader->SetFloat3("u_Lights[0].direction", glm::vec3(1, 1, -1));
	//m_Shader->SetFloat3("u_Lights[0].ambient", glm::vec3(0.7f));
	//m_Shader->SetFloat3("u_Lights[0].diffuse", glm::vec3(0.8f));
	//m_Shader->SetFloat3("u_Lights[0].specular", glm::vec3(0.8f));

	m_UIWindow->Update(ts);

	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();

	Kinematics::RenderCommand::DisableDepthTest();
	Kinematics::RenderCommand::EnableAlphaBlending();
	
	Kinematics::Renderer2D::BeginScene(m_UICamera.GetCamera());
	
	m_UIWindow->Draw(m_UICamera.GetCamera());

	Kinematics::Renderer2D::EndScene();

	Kinematics::RenderCommand::DisableAlphaBlending();
	Kinematics::RenderCommand::EnableDepthTest();

	//m_UIWindow->ApplyLayout();
	/*Kinematics::Renderer::BeginScene(m_Camera);
	Kinematics::Renderer::Submit(m_ModelPlayer, glm::vec3(0, 0, -10.0f), m_Shader);
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			Kinematics::Renderer::Submit(m_Model, glm::vec3(i - 8, j - 5, -20.0f), m_Shader);
		}
	}

	Kinematics::Renderer::EndScene();*/
}

void GameLayer::OnEvent(Kinematics::Event& e)
{
	Kinematics::EventDispatcher dispatcher(e);

	m_UIWindow->OnEvent(e);

	dispatcher.Dispatch<Kinematics::KeyPressedEvent>([this](Kinematics::KeyPressedEvent& e) {

		switch (e.GetKeyCode())
		{
		case KINEMATICS_KEY_UP:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.f, -1.f, 0.f));
			break;
		case KINEMATICS_KEY_DOWN:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.f, 1.f, 0.f));
			break;
		case KINEMATICS_KEY_LEFT:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(1.f, 0.f, 0.f));
			break;
		case KINEMATICS_KEY_RIGHT:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(-1.f, 0.f, 0.f));
			break;
		case KINEMATICS_KEY_LEFT_SHIFT:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.f, 0.f, 1.f));
			break;
		case KINEMATICS_KEY_LEFT_CONTROL:
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.f, 0.f, -1.f));
			break;
		}

		return false;
	});

	dispatcher.Dispatch<Kinematics::WindowResizeEvent>([this](Kinematics::WindowResizeEvent& e) {
		m_UIWindow->SetSize(glm::vec2(e.GetWidth(), e.GetHeight()));
		m_UIWindow->ApplyLayout();
		return false;
	});
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
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
	Kinematics::UI::Window m_UIWindow;
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
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseMoved, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::MouseButtonReleased, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));

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
	
	auto layout = Kinematics::CreateRef<Kinematics::BoxLayout>();
	auto box = Kinematics::CreateRef<Kinematics::Container>();
	auto box2 = Kinematics::CreateRef<Kinematics::Container>();

	box->SetFixedSize({ 200, 200 });
	box2->SetFixedSize({ 50, 20 });

	box->SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	box->SetHighlightColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	box->SetClickColor(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));

	box2->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	box2->SetHighlightColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	box2->SetClickColor(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));

	layout->SetPadding(4);
	
	m_UIWindow.SetLayout(layout);
	m_UIWindow.SetBodyLayout(layout);
	m_UIWindow.PushChild(box);
	m_UIWindow.PushChild(box2);
	m_UIWindow.SetWeight({ 1.f, 1.f });
	m_UIWindow.ApplyLayout();
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

	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();

	Kinematics::RenderCommand::DisableDepthTest();
	Kinematics::Renderer2D::BeginScene(m_UICamera.GetCamera());
	/*for (int i = 0; i < 17; i++)
	{
		Kinematics::Renderer2D::DrawQuad(glm::vec3(i - 8, 0.0f, -20.0f), { 1.0f, 1.0f }, Kinematics::Resources::Get<Kinematics::Texture2D>("0"));
	}*/
	m_UIWindow.Draw(m_UICamera.GetCamera());
	m_UIWindow.ApplyLayout();
	Kinematics::Renderer2D::EndScene();
	Kinematics::RenderCommand::EnableDepthTest();

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

	m_UIWindow.OnEvent(e);

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
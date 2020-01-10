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
	Kinematics::Ref<Kinematics::Shader> m_Shader;
	Kinematics::PerspectiveCamera m_Camera;
};

GameLayer::GameLayer()
	: Layer("GameLayer"), m_Camera(30.0f, 640.f/480.f, 0.1f, 20.0f)
{
}

void GameLayer::OnAttach()
{
	Kinematics::Application::Get().GetFramework()->AddSubSystem("WindowSubSystem");

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

	m_Shader = Kinematics::Shader::Create("assets/shaders/Texture.glsl");	
	m_Mesh = Kinematics::Model::Load("assets/models/chair.obj")->GetMeshes()[0];
}

void GameLayer::OnDetach()
{
}

float sum = 0;

void GameLayer::OnUpdate(Kinematics::Timestep ts)
{	
	sum += ts.GetSeconds();
	m_Shader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(sum*5), glm::vec3(1, 1, 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
	m_Shader->SetInt("u_LightCount", 1);
	m_Shader->SetFloat3("u_ViewerPos", m_Camera.GetPosition());
	m_Shader->SetFloat3("u_Lights[0].direction", glm::vec3(-1, 0, 1));
	m_Shader->SetFloat3("u_Lights[0].ambient",   glm::vec3(1));
	m_Shader->SetFloat3("u_Lights[0].diffuse",   glm::vec3(1));
	m_Shader->SetFloat3("u_Lights[0].specular",  glm::vec3(1));
	
	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();
	
	Kinematics::Renderer::BeginScene(m_Camera);
	Kinematics::Renderer::Submit(m_Mesh, m_Shader, 1);
	Kinematics::Renderer::EndScene();
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
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
	Kinematics::Ref<Kinematics::Shader> m_Shader;
	Kinematics::PerspectiveCamera m_Camera;
};

GameLayer::GameLayer()
	: Layer("GameLayer"), m_Camera(glm::degrees(2 * std::atan2(0.000275, 0.001f)), 8.5f / 5.5f, 0.001f, 30.0f)
{
}

class ParallelFor : public Kinematics::TaskInterface
{
public:
	ParallelFor(size_t count, size_t offset = 0)
		: m_Offset(offset), m_Count(count)
	{
		KINEMATICS_TRACE("PARALLEL FOR Index: {}, Count: {}", m_Offset, m_Count);
	}

	virtual void Run() override
	{
		auto end = m_Offset + m_Count;
		for (size_t i = m_Offset; i < end; ++i)
		{
			KINEMATICS_TRACE("{}", i);
		}
	}

	virtual uint32_t Splittable() override
	{
		return m_Count / 125;
	}

	virtual std::list<Kinematics::Ref<Kinematics::TaskInterface>> Split(uint32_t subTasks)
	{
		std::list< Kinematics::Ref<Kinematics::TaskInterface>> tasks;

		if (m_Count % subTasks == 0)
		{
			for (int i = 0; i < subTasks; i++)
			{
				tasks.push_back(Kinematics::CreateRef<ParallelFor>(m_Count / subTasks, m_Count / subTasks * i));
			}
		}
		else
		{
			tasks.push_back(Kinematics::CreateRef<ParallelFor>(m_Count / subTasks + 1, 0));
			for (int i = 1; i < subTasks; i++)
			{
				tasks.push_back(Kinematics::CreateRef<ParallelFor>(m_Count / subTasks, m_Count / subTasks * i + 1));
			}
		}

		return tasks;
	}

private:
	size_t m_Offset;
	size_t m_Count;
};

class PrintTask : public Kinematics::TaskInterface
{
	virtual void Run() override { KINEMATICS_TRACE("Teste"); }
};

void GameLayer::OnAttach()
{
	Kinematics::Application::Get().GetFramework()->AddSubSystem("WindowSubSystem");
	Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::KeyPressed, KINEMATICS_BIND_EVENT_FN(GameLayer::OnEvent));

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
	m_Mesh = m_Model->GetMeshes()[0];


	Kinematics::Resources::Add("0", Kinematics::Texture2D::Create("assets/textures/0.png", Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE, Kinematics::WrappingOption::KINEMATICS_CLAMP_TO_EDGE));
	auto handle = Kinematics::TaskManager::GetInstance()->Schedule(Kinematics::CreateRef<ParallelFor>(1000));
	Kinematics::TaskManager::GetInstance()->Schedule(Kinematics::CreateRef<PrintTask>(), handle);
	Kinematics::TaskManager::GetInstance()->WaitRunningTaskComplete();
}

void GameLayer::OnDetach()
{
}

float sum = 0;

void GameLayer::OnUpdate(Kinematics::Timestep ts)
{
	sum += ts.GetSeconds();
	m_Shader->SetInt("u_LightCount", 1);
	m_Shader->SetFloat3("u_ViewerPos", m_Camera.GetPosition());
	m_Shader->SetFloat3("u_Lights[0].direction", glm::vec3(1, 1, -1));
	m_Shader->SetFloat3("u_Lights[0].ambient", glm::vec3(0.7f));
	m_Shader->SetFloat3("u_Lights[0].diffuse", glm::vec3(0.8f));
	m_Shader->SetFloat3("u_Lights[0].specular", glm::vec3(0.8f));

	Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Kinematics::RenderCommand::Clear();

	/*Kinematics::Renderer2D::BeginScene(m_Camera);
	for (int i = 0; i < 17; i++)
	{
		Kinematics::Renderer2D::DrawQuad(glm::vec3(i - 8, 0.0f, -20.0f), { 1.0f, 1.0f }, Kinematics::Resources::Get<Kinematics::Texture2D>("0"));
	}
	Kinematics::Renderer2D::EndScene();*/

	Kinematics::Renderer::BeginScene(m_Camera);
	Kinematics::Renderer::Submit(m_Model, glm::vec3(0, 5.0f, -20.0f), m_Shader);
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			Kinematics::Renderer::Submit(m_Model, glm::vec3(i - 8, j - 5, -20.0f), m_Shader);
		}
	}
	Kinematics::Renderer::EndScene();
}

void GameLayer::OnEvent(Kinematics::Event& e)
{
	Kinematics::EventDispatcher dispatcher(e);

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
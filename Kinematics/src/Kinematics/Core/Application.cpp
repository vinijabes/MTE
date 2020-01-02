#include "mtepch.h"
#include "Application.h"

#include "Kinematics/Renderer/Renderer.h"

#include <GLFW/glfw3.h>


namespace Kinematics {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		KINEMATICS_CORE_ASSERT(!s_Instance, "Application Already Exists");
		s_Instance = this;
		m_Framework = CreateRef<Framework>();
		m_Framework->Initialize();

		Kinematics::StateManager::GetInstance()->On(EventType::WindowClose, KINEMATICS_BIND_EVENT_FN(Application::OnEvent));
		Kinematics::StateManager::GetInstance()->On(EventType::WindowResize, KINEMATICS_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{

	}

	bool Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(KINEMATICS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(KINEMATICS_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

		return false;
	}

	void Application::Run()
	{				
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}			

			m_Framework->Update(timestep);
		}
		m_Framework->Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 && e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
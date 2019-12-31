#include "mtepch.h"
#include "WindowSubSystem.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Kinematics/Renderer/Renderer.h"
#include "Kinematics/Renderer/Renderer2D.h"

#include "Kinematics/Core/Timer.h"

#include "Kinematics/Framework/Managers/StateManager.h"


namespace Kinematics {

	SUBSYSTEM_CONSTRUCTOR WindowSubSystemInterface::s_WindowSubSystemInterfaceFactory = [] {return CreateRef<WindowSubSystem>(); };

	void WindowSubSystem::Initialize()
	{
		m_Data.Title = "Kinematics";
		m_Data.Width = 640;
		m_Data.Height = 480;
		m_Data.VSync = true;

		int success = glfwInit();
		KINEMATICS_CORE_ASSERT(success, "Could not initialiaze GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Data.VSync);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			StateManager::GetInstance()->Emit(new WindowCloseEvent());
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			StateManager::GetInstance()->Emit(new WindowResizeEvent(width, height));
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
			StateManager::GetInstance()->Emit(new MouseMovedEvent(x, y));
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				StateManager::GetInstance()->Emit(new KeyPressedEvent(key, 0));
				break;
			}
			case GLFW_RELEASE:
			{
				StateManager::GetInstance()->Emit(new KeyReleasedEvent(key));
				break;
			}
			case GLFW_REPEAT:
			{
				StateManager::GetInstance()->Emit(new KeyPressedEvent(key, 1));
				break;
			}
			}
			});

		Renderer::Init();

		StateManager::GetInstance()->On(EventType::WindowResize, [=](Event& e) {
			this->OnEvent(e);
			return false;
			});
	};

	void WindowSubSystem::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	};

	void WindowSubSystem::Update(Timestep ts)
	{
		KINEMATICS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();

		m_CameraController.OnUpdate(ts);

		{
			KINEMATICS_PROFILE_SCOPE("Prepare to Render");
			Kinematics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Kinematics::RenderCommand::Clear();
		}

		{
			KINEMATICS_PROFILE_SCOPE("Rendering");
			Renderer2D::BeginScene(m_CameraController.GetCamera());

			float aspect = (float)m_Data.Width / m_Data.Height;
			float tileSize = (aspect * 2) / (m_Data.Width/64);

			float verticalSize = 2.0f / (m_Data.Height / 64);

			for (auto obj : m_Objects)
			{
				Renderer2D::DrawQuad({ obj.first * tileSize - tileSize/2, obj.second * verticalSize }, { tileSize, verticalSize }, { 0.8f, 0.2f, 0.3f, 1.0f });				
			}
			Renderer2D::EndScene();
		}
	}
	void WindowSubSystem::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = true;
	}

	bool WindowSubSystem::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowSubSystem::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KINEMATICS_BIND_EVENT_FN(WindowSubSystem::OnWindowResize));
	}

	bool WindowSubSystem::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;
	}

}
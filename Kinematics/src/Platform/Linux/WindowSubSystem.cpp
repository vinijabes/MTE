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
			Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
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
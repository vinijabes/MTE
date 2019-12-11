#include "mtepch.h"
#include "WindowSubSystem.h"

namespace Kinematics {

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
		KINEMATICS_ASSERT(m_Window, "Failed to create window!");

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		KINEMATICS_CORE_ASSERT(status, "Failed to initialize Glad!");

		KINEMATICS_CORE_INFO("OpenGL Info:");
		KINEMATICS_CORE_INFO(" Vendor: {0}", glGetString(GL_VENDOR));
		KINEMATICS_CORE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
		KINEMATICS_CORE_INFO(" Version: {0}", glGetString(GL_VERSION));

		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		glViewport(0, 0, width, height);

		glfwSetWindowUserPointer(m_Window, &m_Data);
	};

	void WindowSubSystem::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	};

	void WindowSubSystem::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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

}
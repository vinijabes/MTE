#pragma once
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Kinematics {

	static void GLFWErrorCallback(int error, const char* description)
	{
		KINEMATICS_CORE_ERROR("GLFW ERROR {0}: {1}", error, description);
	}

	class WindowSubSystem : public SubSystemInterface
	{
	public:
		WindowSubSystem()
		{
			KINEMATICS_TRACE("{0} Instantiated!", this->GetName());
		}
		~WindowSubSystem() {}

		virtual void Install() {};
		virtual void Uninstall() {};

		virtual void Initialize();
		virtual void Shutdown();

		virtual void Update();
		virtual std::vector<std::string> GetDependencies()
		{
			return {};
		}

		void SetWindowCloseCallback(GLFWwindowclosefun fun) { glfwSetWindowCloseCallback(m_Window, fun); }

		void SetVSync(bool enabled);
		bool IsVSync() const;

		virtual void SendMessage(std::string name, void* content)
		{
			if (name == "Close")
			{
				m_Data.closeCb = ((std::function<void()>*)(content));
				glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					if (data.closeCb)
					{
						std::function<void()>* cb = data.closeCb;
						(*cb)();
					}
						
				});
			}
		}

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);

	private:
		GLFWwindow* m_Window;

	private:

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			std::function<void()> *closeCb;
		};

		WindowData m_Data;
	};

	CREATE_FACTORY("WindowSubSystem", WindowSubSystem);
}
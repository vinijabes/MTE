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

		void SetVSync(bool enabled);
		bool IsVSync() const;

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);

	private:
		GLFWwindow* m_Window;

	private:

		struct WindowData 
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};

	CREATE_FACTORY("WindowSubSystem", WindowSubSystem);
}
#pragma once
#include "Kinematics/Framework/Interface/WindowSubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Kinematics {

	static void GLFWErrorCallback(int error, const char* description)
	{
		KINEMATICS_CORE_ERROR("GLFW ERROR {0}: {1}", error, description);
	}

	class WindowSubSystem : public WindowSubSystemInterface
	{
	public:
		WindowSubSystem()
		{
			KINEMATICS_TRACE("{0} Instantiated!", this->GetName());
		}
		~WindowSubSystem() {}

		virtual void Install() override {};
		virtual void Uninstall() override {};

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void Update() override;
		virtual std::vector<std::string> GetDependencies() override
		{
			return {};
		}

		void SetWindowCloseCallback(WindowInputCallback fun) override;
		void SetWindowResizeCallback(WindowInputCallback fun) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);

	private:
		GLFWwindow* m_Window;

	private:

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			WindowInputCallback closeCb;
			WindowInputCallback resizeCb;
		};

		WindowData m_Data;
	};

	CREATE_FACTORY("WindowSubSystem", WindowSubSystem);
}
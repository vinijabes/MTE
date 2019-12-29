#pragma once
#include "Kinematics/Framework/Interface/WindowSubSystemInterface.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Kinematics/Renderer/OrthographicCameraController.h"

namespace Kinematics {

	static void GLFWErrorCallback(int error, const char* description)
	{
		KINEMATICS_CORE_ERROR("GLFW ERROR {0}: {1}", error, description);
	}

	class WindowSubSystem : public WindowSubSystemInterface
	{
	public:
		WindowSubSystem()
			: m_CameraController(640.0f / 480.0f, true)
		{
			KINEMATICS_TRACE("{0} Instantiated!", this->GetName());
		}
		~WindowSubSystem() {}

		virtual void Install() override {};
		virtual void Uninstall() override {};

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(Timestep ts) override;
		virtual std::vector<std::string> GetDependencies() override
		{
			return {};
		}

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

	private:
		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);

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

		GLuint VertexArrayID;
		GLuint vertexbuffer;

		OrthographicCameraController m_CameraController;
	};

	//CREATE_FACTORY("WindowSubSystem", WindowSubSystem);
}
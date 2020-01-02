#include <iostream>

#include "Core.h"
#include "Kinematics/Framework/Events/ApplicationEvent.h"
#include "Kinematics/Core/LayerStack.h"
#include "Kinematics/Framework/Framework.h"

namespace Kinematics {
	class Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();
		bool OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Ref<Framework> GetFramework() { return m_Framework; }
	protected:
		Ref<Framework> m_Framework;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:		

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
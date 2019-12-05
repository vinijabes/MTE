#include <iostream>

#include "Core.h"

namespace Kinematics {
	class Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();
		void Stop() { 
			this->m_Running = false; 
		};

	private:
		bool m_Running = true;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
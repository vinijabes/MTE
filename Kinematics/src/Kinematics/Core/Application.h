#include <iostream>

#include "Core.h"

namespace Kinematics {
	class Application
	{
	public:
		Application();

		virtual ~Application();

		virtual void Run() = 0;
		virtual void Stop() = 0;

		static inline Application& Get() { return *s_Instance; }
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
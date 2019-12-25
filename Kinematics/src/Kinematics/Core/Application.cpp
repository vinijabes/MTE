#include "mtepch.h"
#include "Application.h"

namespace Kinematics {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		KINEMATICS_CORE_ASSERT(!s_Instance, "Application Already Exists");
		s_Instance = this;
	}

	Application::~Application()
	{

	}
}
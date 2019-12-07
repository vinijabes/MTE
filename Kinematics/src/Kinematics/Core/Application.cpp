#include "mtepch.h"
#include "Application.h"
#include "Kinematics/Semaphore.h"
#include "Kinematics/Bus/MessageBus.h"
#include "Kinematics/Bus/BusNode.h"
#include "Kinematics/Framework/Framework.h"
#include <atomic>
#include <thread>
#include <Windows.h>
#include <conio.h>

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
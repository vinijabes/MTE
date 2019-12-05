#include "mtepch.h"
#include "Framework.h"
#include "TaskManager.h"

namespace Kinematics {

	void Framework::Initialize()
	{
		TaskManager::GetInstance()->SetThreadCount(std::thread::hardware_concurrency());
		TaskManager::GetInstance()->Initialize();
		TaskManager::GetInstance()->AddTask("Teste1");
		TaskManager::GetInstance()->AddTask("Teste2");
		TaskManager::GetInstance()->AddTask("Teste3");
		TaskManager::GetInstance()->AddTask("Teste4");
	}

	void Framework::Run()
	{
		while (m_Running)
		{

		}
	}

	void Framework::Stop()
	{
		m_Running = false;
	}

	void Framework::Shutdown()
	{
		TaskManager::GetInstance()->Shutdown();
	}
}
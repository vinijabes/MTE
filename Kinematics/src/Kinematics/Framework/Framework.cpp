#include "mtepch.h"
#include "Framework.h"
#include "Systems/GraphicsSubSystem.h"
#include "Kinematics/Framework/Managers/TaskManager.h"

namespace Kinematics {
	void Framework::Initialize()
	{
		TaskManager::GetInstance()->SetThreadCount(std::thread::hardware_concurrency());
		TaskManager::GetInstance()->Initialize();

		this->AddSubSystem("GraphicsSubSystem");
	}

	void Framework::Update()
	{
		for (auto subSystem : m_SubSystems) subSystem.second->Update();

		TaskManager::GetInstance()->WaitRunningTaskComplete();
	}

	void Framework::Shutdown()
	{
		TaskManager::GetInstance()->Shutdown();
	}
}
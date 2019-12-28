#include "mtepch.h"
#include "Framework.h"
#include "Kinematics/Framework/Managers/TaskManager.h"
#include "Kinematics/Core/Timer.h"
#include "Kinematics/Core/Timestep.h"

#include "GLFW/glfw3.h"


namespace Kinematics {
	void Framework::Initialize()
	{
		TaskManager::GetInstance()->SetThreadCount(std::thread::hardware_concurrency());
		TaskManager::GetInstance()->Initialize();
	}

	void Framework::Update()
	{
		StateManager::GetInstance()->NotifyAll();

		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		{
			/*READ PHASE*/
			for (auto subSystem : m_SubSystems)
				subSystem.second->PreSchedule();
		}

		{
			/*SCHEDULING ALL TASKS*/
			for (auto subSystem : m_SubSystems)
				subSystem.second->Schedule();
		}

		{
			/*CALLED JUST BEFORE UPDATE*/
			for (auto subSystem : m_SubSystems)
				subSystem.second->PreUpdate();
		}

		{
			/*UPDATING ALL STATES AND DOING PROGRAM LOGIC*/
			for (auto subSystem : m_SubSystems)
				subSystem.second->Update(timestep);
		}

		{
			/*CALLED JUST AFTER UPDATE*/
			for (auto subSystem : m_SubSystems)
				subSystem.second->PostUpdate();
		}

		{
			TaskManager::GetInstance()->WaitRunningTaskComplete();
		}
	}

	void Framework::Shutdown()
	{
		for (auto subSystem : m_SubSystems) subSystem.second->Shutdown();
		TaskManager::GetInstance()->Shutdown();
	}
}
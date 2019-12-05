#include "mtepch.h"
#include "TaskManager.h"

namespace Kinematics {
	TaskManager* TaskManager::m_Instance = NULL;

	void TaskManager::Initialize()
	{
		for (int i = 0; i < this->m_ThreadCount; i++)
		{
			this->Push();
		}

		m_Running = true;
	}

	void TaskManager::Shutdown()
	{
		m_Running = false;

		m_Condition.notify_all();

		for (int i = 0; i < m_Pool.size(); i++)
		{
			m_Pool[i].join();
		}
	}

	void TaskManager::Push()
	{
		KINEMATICS_TRACE("PUSHING IN NEW THREAD (NUMBER {})", m_Pool.size() + 1);
		this->m_Pool.push_back(std::thread(&TaskManager::ThreadLoop, this));
	}

	void TaskManager::ThreadLoop()
	{
		KINEMATICS_TRACE("RUNNING THREAD: {}", std::this_thread::get_id());
		std::string task;
		while (m_Running)
		{
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				m_Condition.wait(lock, [=] {return !m_Tasks.empty() || !m_Running; });
				if (!m_Tasks.empty())
				{
					task = m_Tasks.front();
					m_Tasks.pop();
				}
			}

			KINEMATICS_TRACE("EXECUTING THASK {} ON THREAD {}", task,  std::this_thread::get_id());
		}
	}
}
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

		while (m_RunningCount > 0 || m_RunningBackgroundCount > 0);

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
		TaskInterface* task = NULL;
		while (m_Running)
		{
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				if (task) 
				{
					if (task->GetType() == TASK_TYPE::ASYNC_TASK)
					{
						m_RunningCount -= 1;
					}
					else if (task->GetType() == TASK_TYPE::BACKGROUND_TASK)
					{
						m_RunningBackgroundCount -= 1;
					}
				}

				task = NULL;

				m_Condition.wait(lock, [=] {return !m_Tasks.empty() || !m_Running; });

				if ((!m_Tasks.empty() || !m_BackgroundTasks.empty()) && m_Running)
				{
					if (m_RunningBackgroundCount < this->m_MaxBackgroundThreads && !m_BackgroundTasks.empty())
					{
						task = m_BackgroundTasks.front();
						m_BackgroundTasks.pop();
						m_RunningBackgroundCount += 1;
					}
					else if (!m_Tasks.empty())
					{
						task = m_Tasks.front();
						m_Tasks.pop();
						m_RunningCount += 1;
					}

				}
			}

			if (task != NULL) {
				task->OnAccepted();
				task->Run();
				task->OnCompleted();
			}
		}
	}
}
#pragma once

#include "Kinematics/Core/Log.h"
#include "Kinematics/Framework/Interface/TaskInterface.h"
#include "Kinematics/Framework/Interface/BackgroundTaskInterface.h"
#include <thread>
#include <atomic>
#include <vector>
#include <queue>
#include <condition_variable>

namespace Kinematics {

	class TaskManager
	{
	public:
		TaskManager(TaskManager const&) = delete;
		void operator=(TaskManager const&) = delete;

		static TaskManager* GetInstance()
		{
			if (!m_Instance)
			{
				m_Instance = new TaskManager();
			}

			return m_Instance;
		}

		void SetThreadCount(int count) 
		{ 
			this->m_ThreadCount = count; 
			this->m_MaxBackgroundThreads = std::max(count / 4, 1);
		}

		void Initialize();
		void Shutdown();

		void AddTask(TaskInterface* task)
		{
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				if (task->GetType() == TASK_TYPE::ASYNC_TASK)
					m_Tasks.push(task);
				else if (task->GetType() == TASK_TYPE::BACKGROUND_TASK)
					m_BackgroundTasks.push(task);
			}

			m_Condition.notify_one();
		}

		void WaitRunningTaskComplete()
		{
			while (m_RunningCount > 0);
		}

	private:
		TaskManager()
			: m_ThreadCount(std::thread::hardware_concurrency()),
			m_MaxBackgroundThreads(std::thread::hardware_concurrency() / 4)
		{}

		void ThreadLoop();
		void Push();

	private:
		static TaskManager* m_Instance;

		int m_ThreadCount;
		int m_MaxBackgroundThreads;
		std::atomic<bool> m_Running;
		std::atomic<int> m_RunningCount = 0;
		std::atomic<int> m_RunningBackgroundCount = 0;

		std::vector<std::thread> m_Pool;
		std::queue<TaskInterface*> m_Tasks;
		std::queue<TaskInterface*> m_BackgroundTasks;

		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
	};
}
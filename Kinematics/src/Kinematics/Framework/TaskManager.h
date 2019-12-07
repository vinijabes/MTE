#pragma once

#include "Kinematics/Core/Log.h"
#include "Kinematics/Framework/Interface/TaskInterface.h"
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

		void SetThreadCount(int count) { this->m_ThreadCount = count; }

		void Initialize();
		void Shutdown();

		void AddTask(TaskInterface* task)
		{
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				m_Tasks.push(task);
			}

			m_Condition.notify_one();
		}

		void WaitRunningTaskComplete()
		{
			while (m_RunningCount > 0);
		}

	private:
		TaskManager() {}

		void ThreadLoop();
		void Push();

	private:
		static TaskManager* m_Instance;

		int m_ThreadCount;
		std::atomic<bool> m_Running;
		std::atomic<int> m_RunningCount = 0;
		std::vector<std::thread> m_Pool;
		std::queue<TaskInterface*> m_Tasks;

		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
	};
}
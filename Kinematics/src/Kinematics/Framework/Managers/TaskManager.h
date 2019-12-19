#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Core/Log.h"
#include "Kinematics/Framework/Async/WorkerThread.h"
#include "Kinematics/Framework/Interface/TaskInterface.h"
#include "Kinematics/Framework/Interface/BackgroundTaskInterface.h"
#include <thread>
#include <atomic>
#include <vector>
#include <deque>
#include <unordered_map>
#include <condition_variable>

namespace Kinematics {

	class TaskManager
	{
	public:
		class TaskHandle
		{
		public:
			TaskHandle(Ref<TaskInterface> task)
				:m_Task(task)
			{
			}

			bool IsCompleted() { return m_Task->m_Completed; }
			bool IsReady()
			{
				for (auto dependencie : m_Dependencies)
					if (!dependencie->IsCompleted()) return false;

				return true;
			}
		private:
			friend class TaskManager;

			void AddDependency(Ref<TaskHandle> dependency) { 
				m_Dependencies.push_back(dependency); 
			}
			Ref<TaskInterface> GetTask() const { return m_Task; }

		private:
			Ref<TaskInterface> m_Task;
			std::vector<Ref<TaskHandle>> m_Dependencies;
		};


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

		Ref<TaskInterface> GiveTask();
		void CompleteTask(Ref<TaskInterface> task);

		Ref<TaskHandle> Schedule(Ref<TaskInterface> task);
		Ref<TaskHandle> Schedule(Ref<TaskInterface> task, Ref<TaskHandle> dependencie);

		void ReSchedule(Ref<TaskHandle> task);
		void ReScheduleBack(Ref<TaskHandle> task);

		void WaitRunningTaskComplete()
		{
			while (m_TaskDeque.size() || m_RunningCount > 0);

			KINEMATICS_ASSERT(m_TaskDeque.size() == 0, "Deque of tasks should be empty!");
			m_TaskDeque.clear();
			m_HandleMap.clear();
		}

	private:
		TaskManager()
			: m_ThreadCount(std::thread::hardware_concurrency()),
			m_MaxBackgroundThreads(std::thread::hardware_concurrency() / 4)
		{
		}

		void ThreadLoop();
		void Push();

		Ref<TaskHandle> PopTask();

	private:
		static TaskManager* m_Instance;

		int m_ThreadCount;
		int m_MaxBackgroundThreads;
		std::atomic<bool> m_Running;
		std::atomic<int> m_RunningCount = 0;
		std::atomic<int> m_RunningBackgroundCount = 0;

		std::vector<std::thread> m_Pool;
		std::vector<WorkerThread*> m_Workers;
		std::deque<Ref<TaskHandle>> m_TaskDeque;
		std::unordered_map<Ref<TaskInterface>, Ref<TaskHandle>> m_HandleMap;

		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;

	};
}
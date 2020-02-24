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

			void Wait()
			{
				while (!m_Task->m_Completed) std::this_thread::yield();
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
			if (m_Running)
			{
				if(m_ThreadCount > count)
				{
					Shutdown();
					this->m_ThreadCount = count;
					this->m_MaxBackgroundThreads = std::max(count / 4, 1);
					Initialize();
				}
				else
				{
					for (int i = m_ThreadCount; i < count; i++)
					{
						this->Push();
					}
					this->m_ThreadCount = count;
					this->m_MaxBackgroundThreads = std::max(count / 4, 1);
				}
			}
			else
			{
				this->m_ThreadCount = count;
				this->m_MaxBackgroundThreads = std::max(count / 4, 1);
			}
		}

		int GetThreadCount() const { return m_ThreadCount; }

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
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_Finished.wait(lock, [this]() {
				return m_TaskDeque.empty() && m_RunningCount == 0;
				});

			KINEMATICS_ASSERT(m_TaskDeque.size() == 0, "Deque of tasks should be empty!");

			m_HandleMap.clear();	
		}

		void Clear()
		{
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
		std::atomic<int> m_TaskCount = 0;
		std::unordered_map<Ref<TaskInterface>, Ref<TaskHandle>> m_HandleMap;

		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::condition_variable m_Finished;
	};
}
#pragma once

#include "Kinematics/Core/Core.h"
#include <thread>
#include <atomic>

namespace Kinematics {

	class TaskManager;
	class TaskInterface;

	class WorkerThread
	{
	public:
		WorkerThread(TaskManager* taskScheduler, int id);
		~WorkerThread();

		void Run();
		void AcceptTask();
		void CompleteTask();
		void RunTask();
		void Stop() { m_Running = false; }

		bool isIdle() { return !m_Task; }
	private:
		friend class TaskManager;

	private:
		int m_Id;

		Ref<TaskInterface> m_Task;

		TaskManager* m_TaskManager;

		std::atomic<bool> m_Running;
		Scope<std::thread> m_Thread;
	};
}
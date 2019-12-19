#include "mtepch.h"
#include "WorkerThread.h"

#include "Kinematics/Framework/Managers/TaskManager.h"
#include "Kinematics/Framework/Interface/TaskInterface.h"

namespace Kinematics {
	WorkerThread::WorkerThread(TaskManager* taskManager, int id)
		: m_Running(true), m_TaskManager(taskManager), m_Id(id), m_Thread(new std::thread(&WorkerThread::Run, this))
	{
	}

	WorkerThread::~WorkerThread()
	{
		m_Thread->join();
	}


	void WorkerThread::Run()
	{
		KINEMATICS_INFO("New Thread");
		while (m_Running)
		{
			AcceptTask();
		}
		KINEMATICS_INFO("Thread Stopped");
	}

	void WorkerThread::AcceptTask()
	{
		m_Task = m_TaskManager->GiveTask();
		if (m_Task)
		{
			m_Task->OnAccepted();
			RunTask();
		}
	}

	void WorkerThread::RunTask()
	{
		KINEMATICS_ASSERT(m_Task != nullptr, "Worker must have an task to execute!");
		m_Task->Run();
		CompleteTask();
	}

	void WorkerThread::CompleteTask()
	{
		m_TaskManager->CompleteTask(m_Task);
		m_Task = NULL;
	}
}
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
		for (int i = 0; i < m_Workers.size(); i++)
		{
			m_Workers[i]->Stop();
		}

		m_Running = false;

		m_Condition.notify_all();

		for (int i = 0; i < m_Workers.size(); i++)
		{
			delete m_Workers[i];
		}
	}

	void TaskManager::Push()
	{
		KINEMATICS_TRACE("PUSHING IN NEW THREAD (NUMBER {})", m_Workers.size() + 1);
		this->m_Workers.push_back(new WorkerThread(this, m_Workers.size() + 1));
	}

	Ref<TaskManager::TaskHandle> TaskManager::PopTask()
	{ 
		auto it = m_TaskDeque.begin();
		auto end = m_TaskDeque.end();

		for (; it != end; it++)
		{
			if ((*it)->IsReady())
			{
				Ref<TaskManager::TaskHandle> handle(*it);
				m_TaskDeque.erase(it);
				return handle;
			}
		}

		return nullptr;
	}

	Ref<TaskInterface> TaskManager::GiveTask()
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		m_Condition.wait(lock, [=] {return !m_TaskDeque.empty() || !m_Running; });

		if (!m_TaskDeque.empty() && m_Running)
		{
			Ref<TaskHandle> handle = PopTask();
			if (handle)
			{
				m_RunningCount += 1;
				return handle->GetTask();
			}
		}

		return Ref<TaskInterface>(nullptr);
	}

	void TaskManager::CompleteTask(Ref<TaskInterface> task)
	{
		task->m_Completed = true;
		m_RunningCount -= 1;
		
		if (task->m_Next.size() > 0)
		{
			for (auto nextTask : task->m_Next)
			{
				KINEMATICS_ASSERT(m_HandleMap.find(nextTask) != m_HandleMap.end(), "Children task don't have a Handler!");
				if (m_HandleMap[nextTask]->IsReady())  ReSchedule(m_HandleMap[nextTask]);
			}
		}
	}

	Ref<TaskManager::TaskHandle> TaskManager::Schedule(Ref<TaskInterface> task)
	{
		return Schedule(task, nullptr);
	}

	Ref<TaskManager::TaskHandle> TaskManager::Schedule(Ref<TaskInterface> task, Ref<TaskManager::TaskHandle> dependency)
	{
		auto handle = CreateRef<TaskHandle>(task);
		if (dependency)
		{
			handle->AddDependency(dependency);
			dependency->GetTask()->PushChild(task);
			m_HandleMap[task] = handle;
			return handle;
		}

		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_TaskDeque.push_back(handle);
			m_HandleMap[task] = handle;
		}

		m_Condition.notify_one();
		return handle;
	}

	void TaskManager::ReSchedule(Ref<TaskHandle> task)
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_TaskDeque.push_front(task);
		}

		m_Condition.notify_one();
	}

	void TaskManager::ReScheduleBack(Ref<TaskHandle> task)
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_TaskDeque.push_back(task);
		}

		m_Condition.notify_one();
	}
}
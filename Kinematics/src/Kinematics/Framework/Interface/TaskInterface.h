#pragma once

#include <vector>
#include "Kinematics/Framework/Async/WorkerThread.h"

namespace Kinematics {

#define TASK_CLASS_TYPE(type) static TASK_TYPE GetStaticType() {return TASK_TYPE::type;}\
                              virtual TASK_TYPE GetType() const override {return TASK_TYPE::type;}

	typedef enum
	{
		BACKGROUND_TASK,
		ASYNC_TASK,
		SYNC_TASK
	} TASK_TYPE;

	class TaskInterface
	{
	public:
		TaskInterface() : m_Completed(false) {}
		virtual ~TaskInterface() {};

		virtual TASK_TYPE GetType() const { return TASK_TYPE::ASYNC_TASK; }
		unsigned int GetPriority() { return 0; }

		/*TASK CALLBACKS*/
		virtual void OnAccepted() {};
		virtual void OnCompleted() {};
		virtual void Run() = 0;

		/*TASK SPLITING*/
		virtual uint32_t Splittable() { return 0; }
		virtual std::list<Ref<TaskInterface>> Split(uint32_t count)
		{
			return std::list<Ref<TaskInterface>>();
		};

		bool IsRoot() { return m_RootTask; }
	private:
		void PushChild(Ref<TaskInterface> child)
		{
			if(m_Child.empty())
				m_Next.push_back(child);
			else
				for (auto c : m_Child)
				{
					c->PushChild(child);
				}
		}

		void PushSubTask(Ref<TaskInterface> child)
		{
			m_RootTask = true;
			m_Child.push_back(child);
			m_Next.push_back(child);
		}
	private:
		friend class TaskManager;
		std::vector<Ref<TaskInterface>> m_Next;
		std::vector<Ref<TaskInterface>> m_Child;
		std::atomic<bool> m_Completed;
		std::atomic<bool> m_RootTask = false;
	};
}
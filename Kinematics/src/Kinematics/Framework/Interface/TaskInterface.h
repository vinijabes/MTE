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
		virtual bool Splittable() { return false; }
		virtual void Split(WorkerThread worker) {};
	private:
		void PushChild(Ref<TaskInterface> child)
		{
			m_Next.push_back(child);
		}
	private:
		friend class TaskManager;
		std::vector<Ref<TaskInterface>> m_Next;
		std::atomic<bool> m_Completed;
	};
}
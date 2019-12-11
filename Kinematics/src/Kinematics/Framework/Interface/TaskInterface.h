#pragma once

namespace Kinematics {

#define TASK_CLASS_TYPE(type) static TASK_TYPE GetStaticType() {return TASK_TYPE::##type;}\
                              virtual TASK_TYPE GetType() const override {return TASK_TYPE::##type;}

	typedef enum
	{
		BACKGROUND_TASK,
		ASYNC_TASK,
		SYNC_TASK
	} TASK_TYPE;

	class TaskInterface
	{
	public:
		virtual ~TaskInterface() {};

		virtual TASK_TYPE GetType() const { return TASK_TYPE::ASYNC_TASK; }
		unsigned int GetPriority() { return 0; }

		virtual void OnAccepted() {};
		virtual void OnCompleted() {};
		virtual void Run() = 0;
	};
}
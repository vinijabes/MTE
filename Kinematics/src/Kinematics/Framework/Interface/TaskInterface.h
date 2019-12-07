#pragma once

namespace Kinematics {

#define TASK_CLASS_TYPE(type) static const char* GetStaticType() {return #type;}\
                              virtual const char* GetType() const override {return TASK_TYPE::##type;}

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

		virtual const char* GetType() const { return "TaskInterface"; }
		unsigned int GetPriority() { return 0; }

		virtual void OnAccepted() {};
		virtual void OnCompleted() {};
		virtual void Run() = 0;
	};
}
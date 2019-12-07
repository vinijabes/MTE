#pragma once

namespace Kinematics {

	class TaskInterface
	{
	public:
		virtual ~TaskInterface() {};

		std::string GetType() { return "TaskInterface"; }
		unsigned int GetPriority() { return 0; }

		virtual void OnAccepted() {};
		virtual void OnCompleted() {};
		virtual void Run() = 0;
	};
}
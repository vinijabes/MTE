#pragma once

#pragma once

#include <atomic>
#include "Kinematics/Framework/Interface/TaskInterface.h"

namespace Kinematics {
	class BackgroundTask : public TaskInterface
	{
	public:
		TASK_CLASS_TYPE(BackgroundTask);

		void Cancel()
		{
			m_Cancelled = true;
			m_RunningTask = false;

			this->OnCancel();
		}

		virtual void OnCancel() {}
		virtual void OnCompleted() override final {
			if (!m_Cancelled) this->OnBackgroundCompleted();
		}
		virtual void OnBackgroundCompleted() {}

		virtual bool Cancellable() const { return true; }
	private:
		std::atomic<bool> m_RunningTask = true;
		std::atomic<bool> m_Cancelled = false;
	};
}
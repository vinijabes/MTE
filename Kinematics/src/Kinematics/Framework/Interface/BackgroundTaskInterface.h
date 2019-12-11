#pragma once

#include <atomic>
#include "Kinematics/Framework/Interface/TaskInterface.h"

namespace Kinematics {
	class BackgroundTask : public TaskInterface
	{
	public:
		TASK_CLASS_TYPE(BACKGROUND_TASK);

		void Cancel()
		{
			if (m_Completed) return;

			m_Cancelled = true;
			m_RunningTask = false;

			this->OnCancel();
		}

		virtual void OnCancel() {}
		virtual void OnCompleted() override final {
			if (!m_Cancelled) {
				this->OnBackgroundCompleted();
				m_Completed = true;
			}
		}
		virtual void OnBackgroundCompleted() {}

		virtual bool Cancellable() const { return true; }

		bool IsCancelled() { return m_Cancelled; }
	private:
		std::atomic<bool> m_RunningTask = true;
		std::atomic<bool> m_Cancelled = false;
		std::atomic<bool> m_Completed = false;
	};
}
#include "mtepch.h"
#include "Semaphore.h"

namespace Kinematics {

	void Semaphore::Down()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (--m_Value < 0)
		{
			m_WaitCondition.wait(lock);
		}
	}

	void Semaphore::Up()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (++m_Value <= 0) m_WaitCondition.notify_one();
	}
}
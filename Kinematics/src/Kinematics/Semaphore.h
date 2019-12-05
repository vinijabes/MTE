#pragma once

#include <mutex>
#include <condition_variable>

namespace Kinematics {
	class Semaphore
	{
	public:
		Semaphore(int init)
			: m_Value(init)
		{

		}

		void Down();
		void Up(); 

		int GetValue() const { return m_Value; }

	private:
		int m_Value;
		std::mutex m_Mutex;
		std::condition_variable m_WaitCondition;
	};
}
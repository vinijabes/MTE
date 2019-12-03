#pragma once

#include <mutex>
#include <condition_variable>

namespace MTE {
	class Semaphore
	{
	public:
		Semaphore(int init)
			: m_Value(init)
		{

		}

		void Down();
		void Up(); 


	private:
		int m_Value;
		std::mutex m_Mutex;
		std::condition_variable m_WaitCondition;
	};
}
#include "mtepch.h"
#include "Application.h"
#include "Semaphore.h"
#include <thread>

namespace MTE {
	
	Semaphore s(5);

	void foo(int Z)
	{
		s.Down();
		for (int i = 0; i < Z; i++)
		{
			std::cout << i << std::endl;
		}
	}

	void Application::Run()
	{

		std::thread th1(foo, 5);
		std::thread th2(foo, 10);
		std::thread th3(foo, 20);
		std::thread th4(foo, 30);
		std::thread th5(foo, 40);
		std::cout << "Running Application" << std::endl;

		th1.join();
		th2.join();
		th3.join();
		th4.join();
		th5.join();

	}
}
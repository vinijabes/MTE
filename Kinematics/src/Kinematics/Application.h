#include <iostream>

namespace MTE {
	class Application
	{
	public:
		Application()
		{

		}

		void Run();
		void Stop() { 
			this->m_Running = false; 
		};

	private:
		bool m_Running;
	};
}
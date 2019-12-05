#include "mtepch.h"
#include "Framework.h"

namespace Kinematics {

	void Framework::Initialize()
	{
	}

	void Framework::Run()
	{
		while (m_Running)
		{

		}
	}

	void Framework::Stop()
	{
		m_Running = false;
	}
}
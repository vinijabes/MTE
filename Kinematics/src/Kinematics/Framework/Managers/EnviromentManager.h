#pragma once
#include <unordered_map>

namespace Kinematics
{

#define KINEMATICS_OPENGL_MAJOR 0u
#define KINEMATICS_OPENGL_MINOR 1u

	class EnviromentManager
	{
	public:
		EnviromentManager(EnviromentManager const&) = delete;
		void operator=(EnviromentManager const&) = delete;

		static EnviromentManager* GetInstance()
		{
			if (!m_Instance)
			{
				m_Instance = new EnviromentManager();
			}

			return m_Instance;
		}

		static void Set(uint32_t key, int value) { GetInstance()->m_Values[key] = value; }
		static uint32_t Get(uint32_t key) { return GetInstance()->m_Values[key]; }
	private:
		EnviromentManager() {}

	private:
		static EnviromentManager* m_Instance;
		std::unordered_map<uint32_t, int> m_Values;
	};
}
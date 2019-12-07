#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <functional>
#include <unordered_map>

namespace Kinematics {

	typedef std::function<SubSystemInterface * ()> SUBSYSTEM_CONSTRUCTOR;

	template <typename T>
	class FactoryRegistrator
	{
	public:
		FactoryRegistrator(const char* id)
		{
			FactoryManager::GetInstance()->RegisterSubSystem(id, []() {return static_cast<SubSystemInterface*>(new T()); });
		}
	};

#define CREATE_FACTORY(id, type) FactoryRegistrator<type> _##type##Factory(id);

	class FactoryManager
	{
	public:
		FactoryManager(FactoryManager const&) = delete;
		void operator=(FactoryManager const&) = delete;

		static FactoryManager* GetInstance()
		{
			if (!m_Instance)
			{
				m_Instance = new FactoryManager();
			}

			return m_Instance;
		}

		void RegisterSubSystem(std::string subsystem, SUBSYSTEM_CONSTRUCTOR subsystemFactory);
		SubSystemInterface* Create(std::string subsystem);
	private:
		FactoryManager() {}

	private:
		static FactoryManager* m_Instance;

		std::unordered_map<std::string, SUBSYSTEM_CONSTRUCTOR> m_FactoryTemplates;
	};
}
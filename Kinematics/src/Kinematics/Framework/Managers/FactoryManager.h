#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <functional>
#include <unordered_map>

namespace Kinematics {

	typedef std::function<Ref<SubSystemInterface>()> SUBSYSTEM_CONSTRUCTOR;

	template <typename T>
	class FactoryRegistrator
	{
	public:
		FactoryRegistrator(const char* id)
		{
			FactoryManager::GetInstance()->RegisterSubSystem(id, []() {return std::static_pointer_cast<SubSystemInterface>( T::Create()); });
		}

		FactoryRegistrator(const char* id, SUBSYSTEM_CONSTRUCTOR fn)
		{
			FactoryManager::GetInstance()->RegisterSubSystem(id, fn);
		}
	};

#define CREATE_FACTORY(id, type) inline extern FactoryRegistrator<type> _##type##Factory(id);

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
		Ref<SubSystemInterface> Create(std::string subsystem);
	private:
		FactoryManager() {}

	private:
		static FactoryManager* m_Instance;

		std::unordered_map<std::string, SUBSYSTEM_CONSTRUCTOR> m_FactoryTemplates;
	};
}
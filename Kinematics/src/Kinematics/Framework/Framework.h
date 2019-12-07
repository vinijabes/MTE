#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/FactoryManager.h"
#include <map>

namespace Kinematics {

	class Framework
	{
	public:
		Framework() {}
		virtual ~Framework() {}

		virtual void Initialize();
		virtual void Shutdown();

		virtual void Update();

		template <class T>
		void AddSubSystem()
		{
			this->AddSubSystem(T::GetStaticName());
		}

		void AddSubSystem(std::string name)
		{
			SubSystemInterface* subSystem = FactoryManager::GetInstance()->Create(name);

			KINEMATICS_ASSERT(subSystem != NULL, "Factory can't instantiate selected subsystem!");
			m_SubSystems[subSystem->GetName()] = subSystem;

			std::vector<std::string> dependencies = subSystem->GetDependencies();

			for (auto dependencie : dependencies)
			{
				if (m_SubSystems.find(dependencie) == m_SubSystems.end())
				{
					this->AddSubSystem(dependencie);
				}
			}
		}

		template <class T>
		void RemoveSubSystem()
		{
			m_SubSystems.erase(T::GetStaticName());
		}

	private:
		std::unordered_map<std::string, SubSystemInterface*> m_SubSystems;
	};

}
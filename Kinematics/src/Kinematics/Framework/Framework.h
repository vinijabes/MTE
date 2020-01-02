#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"
#include <map>

namespace Kinematics {

	class Framework
	{
	public:
		Framework() {}
		virtual ~Framework() {}

		virtual void Initialize();
		virtual void Shutdown();

		virtual void Update(Timestep ts);

		template <class T>
		void AddSubSystem()
		{
			this->AddSubSystem(T::GetStaticName());
		}

		void AddSubSystem(std::string name)
		{
			Ref<SubSystemInterface> subSystem = FactoryManager::GetInstance()->Create(name);
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

			subSystem->Initialize();
		}

		template <class T>
		void RemoveSubSystem()
		{
			m_SubSystems.erase(T::GetStaticName());
		}

		template <class T>
		Ref<T> GetSubSystem()
		{
			return std::dynamic_pointer_cast<T>(m_SubSystems[T::GetStaticName()]);
		}

		Ref<SubSystemInterface> GetSubSystem(std::string name)
		{
			return m_SubSystems[name];
		}

	private:
		std::unordered_map<std::string, Ref<SubSystemInterface>> m_SubSystems;
		float m_LastFrameTime = 0.0f;
	};

}
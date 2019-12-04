#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <map>

namespace Kinematics {

	class Framework
	{
	public:
		Framework() {}
		virtual ~Framework() {}

		virtual void Initialize();
		virtual void Shutdown();

		virtual void Run();
		virtual void Stop();

		template <class T>
		void AddSubSystem()
		{
			SubSystemInterface *subSystem = new T();
			m_SubSystems[T::GetStaticName()] = subSystem;
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
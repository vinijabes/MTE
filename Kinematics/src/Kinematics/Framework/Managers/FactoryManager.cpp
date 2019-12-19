#include "mtepch.h"
#include "FactoryManager.h"

namespace Kinematics {
	FactoryManager* FactoryManager::m_Instance = NULL;

	void FactoryManager::RegisterSubSystem(std::string subsystem, SUBSYSTEM_CONSTRUCTOR subsystemFactory)
	{
		KINEMATICS_CORE_ASSERT(m_FactoryTemplates.find(subsystem) == m_FactoryTemplates.end(), "Can have only Factory with key " + subsystem);

		if (m_FactoryTemplates.find(subsystem) == m_FactoryTemplates.end())
			m_FactoryTemplates[subsystem] = subsystemFactory;
	}

	Ref<SubSystemInterface> FactoryManager::Create(std::string subsystem)
	{
		KINEMATICS_CORE_ASSERT(m_FactoryTemplates.find(subsystem) != m_FactoryTemplates.end(), "Factory of " + subsystem + " is not registered");
		if (m_FactoryTemplates.find(subsystem) == m_FactoryTemplates.end()) return NULL;

		return Ref<SubSystemInterface>(m_FactoryTemplates[subsystem]());
	}
}
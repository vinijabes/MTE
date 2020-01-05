#pragma once
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"


namespace Kinematics {
	class ScriptSubSystemInterface : public SubSystemInterface
	{
	public:
		SUBSYSTEM_CLASS_TYPE(ScriptSubSystem);

		static Ref<ScriptSubSystemInterface> Create() { return nullptr; }
	};

	CREATE_FACTORY("ScriptSubSystem", ScriptSubSystemInterface);
}
#pragma once
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"

namespace Kinematics {
	class GraphicsSubSystem : public SubSystemInterface
	{
	public:
		GraphicsSubSystem()
		{
			KINEMATICS_TRACE("{0} Instantiated!", this->GetName());
		}
		~GraphicsSubSystem() {}

		virtual void Install() {};
		virtual void Uninstall() {};

		virtual void Initialize() {};
		virtual void Shutdown() {};

		virtual void Update() {};

		virtual std::vector<std::string> GetDependencies()
		{
			return { "WindowSubSystem" };
		}

		SUBSYSTEM_CLASS_TYPE(GraphicsSubSystem);
	};

	CREATE_FACTORY("GraphicsSubSystem", GraphicsSubSystem);
}
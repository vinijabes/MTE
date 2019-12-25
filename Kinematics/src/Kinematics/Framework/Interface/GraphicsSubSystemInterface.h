#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"

namespace Kinematics {
	class GraphicsSubSystemInterface : public SubSystemInterface
	{
	public:
		SUBSYSTEM_CLASS_TYPE(GraphicsSubSystem);

		static Ref<GraphicsSubSystemInterface> Create() { return std::static_pointer_cast<GraphicsSubSystemInterface>(s_GraphicsSubSystemInterfaceFactory()); };
	private:
		static SUBSYSTEM_CONSTRUCTOR s_GraphicsSubSystemInterfaceFactory;
	};

	CREATE_FACTORY("GraphicsSubSystem", GraphicsSubSystemInterface);
}
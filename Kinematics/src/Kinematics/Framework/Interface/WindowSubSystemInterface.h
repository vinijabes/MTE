#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"
#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Renderer/GraphicsContext.h"
#include "Kinematics/Core/Hash.h"

#include "Kinematics/Framework/Events/ApplicationEvent.h"
#include "Kinematics/Framework/Events/MouseEvent.h"

namespace Kinematics {

	typedef std::function<void(Event&)> WindowInputCallback;
	class WindowSubSystemInterface : public SubSystemInterface
	{
	public:
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);

		static Ref<WindowSubSystemInterface> Create() { return std::static_pointer_cast<WindowSubSystemInterface>(s_WindowSubSystemInterfaceFactory()); };
	private:
		static SUBSYSTEM_CONSTRUCTOR s_WindowSubSystemInterfaceFactory;

	protected:
		Scope<GraphicsContext> m_Context;
	};

	CREATE_FACTORY("WindowSubSystem", WindowSubSystemInterface);
}
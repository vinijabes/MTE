#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"
#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Renderer/GraphicsContext.h"

#include "Kinematics/Framework/Events/ApplicationEvent.h"
#include "Kinematics/Framework/Events/MouseEvent.h"
#include "Kinematics/Framework/Events/KeyEvent.h"

#include <list>
#include <glm/glm.hpp>

namespace Kinematics {

	typedef std::function<void(Event&)> WindowInputCallback;
	class WindowSubSystemInterface : public SubSystemInterface
	{
	public:
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);
		void Submit(int16_t x, int16_t y) { m_Objects.push_back(std::make_pair(x, y)); };
		void ClearList() { m_Objects.clear(); }

		virtual inline unsigned int GetWidth() const = 0;
		virtual inline unsigned int GetHeight() const = 0;

		virtual inline glm::vec2 GetMousePos() const = 0;

		static Ref<WindowSubSystemInterface> Create() { return std::static_pointer_cast<WindowSubSystemInterface>(s_WindowSubSystemInterfaceFactory()); };
	private:
		static SUBSYSTEM_CONSTRUCTOR s_WindowSubSystemInterfaceFactory;

	protected:
		Scope<GraphicsContext> m_Context;
		std::list<std::pair<int16_t, int16_t>> m_Objects;
	};

	CREATE_FACTORY("WindowSubSystem", WindowSubSystemInterface);
}
#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Framework/Events/Event.h"

#include "Kinematics/Core/Timestep.h"

namespace Kinematics {
	class KINEMATICS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}
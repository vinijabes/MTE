#pragma once

#include <string>
#include <vector>

#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Core/Timestep.h"

namespace Kinematics {

#define SUBSYSTEM_CLASS_TYPE(type) static const char* GetStaticName() {return #type;}\
                                   virtual const char* GetName() const override { return #type;}


	class SubSystemInterface
	{
	public:
		virtual ~SubSystemInterface() {};

		virtual const char* GetName() const = 0;

		virtual void Install() = 0;
		virtual void Uninstall() = 0;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void PreSchedule() {};
		virtual void Schedule() {};
		virtual void PreUpdate() {};
		virtual void Update(Timestep ts) = 0;
		virtual void PostUpdate() {};

		virtual void OnEvent(Event& e) {};

		virtual std::vector<std::string> GetDependencies() = 0;

		virtual void SendMessage(std::string name, void* content) {}
	};
}
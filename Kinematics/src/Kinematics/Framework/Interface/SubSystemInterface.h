#pragma once

#include <string>
#include <vector>

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

		virtual void Update() = 0;

		virtual std::vector<std::string> GetDependencies() = 0;
	};
}
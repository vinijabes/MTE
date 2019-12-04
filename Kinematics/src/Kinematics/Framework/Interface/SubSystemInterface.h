#pragma once

#include <string>

namespace Kinematics {
	class SubSystemInterface
	{
	public:
		~SubSystemInterface();

		virtual const std::string GetName() const = 0;

		virtual void Install() = 0;
		virtual void Uninstall() = 0;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Update() = 0;
	};
}
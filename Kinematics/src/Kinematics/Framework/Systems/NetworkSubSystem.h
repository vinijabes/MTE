#pragma once

#include "Kinematics/Framework/Interface/NetworkSubSystemInterface.h"

namespace Kinematics {
	class NetworkSubSystem : public NetworkSubSystemInterface
	{
		virtual void Install() override;
		virtual void Uninstall() override;

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(Timestep ts) override;

		virtual std::vector<std::string> GetDependencies() override;

		virtual void Listen(uint32_t port) override;
		virtual void Connect(const char* ip, uint32_t port) override;
	};
}
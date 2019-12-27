#pragma once

#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Network/Socket.h"

#include "SubSystemInterface.h"

namespace Kinematics {
	class NetworkSubSystemInterface : public SubSystemInterface
	{
	public:
		virtual void Listen(uint32_t port) = 0;
		virtual void Connect(const char* ip, uint32_t port) = 0;

		SUBSYSTEM_CLASS_TYPE(NetworkSubSystem);

		static Ref<NetworkSubSystemInterface> Create() { return std::static_pointer_cast<NetworkSubSystemInterface>(s_NetworkSubSystemInterfaceFactory()); };
	protected:
		Scope<Socket> m_ServerSocket;
		Scope<Socket> m_ClientSocket;

		uint16_t m_ConnectionLimit;

	private:
		static SUBSYSTEM_CONSTRUCTOR s_NetworkSubSystemInterfaceFactory;
	};

	CREATE_FACTORY("NetworkSubSystem", NetworkSubSystemInterface);
}
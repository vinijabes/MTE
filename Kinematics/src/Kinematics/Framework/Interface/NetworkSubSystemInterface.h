#pragma once

#include "Kinematics/Framework/Managers/FactoryManager.h"

#include "SubSystemInterface.h"

#ifdef KINEMATICS_PLATFORM_WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>

using KinematicsSocket = SOCKET;
#define KINEMATICS_INVALID_SOCKET INVALID_SOCKET

#endif

namespace Kinematics {
	class NetworkSubSystemInterface : public SubSystemInterface
	{
	public:
		virtual void Listen(uint32_t port) = 0;
		virtual void Connect(const char *ip, uint32_t port) = 0;

		SUBSYSTEM_CLASS_TYPE(NetworkSubSystem);

		static Ref<NetworkSubSystemInterface> Create() { return std::static_pointer_cast<NetworkSubSystemInterface>(s_NetworkSubSystemInterfaceFactory()); };
	protected:
		KinematicsSocket m_ServerSocket;
		KinematicsSocket m_ClientSocket;

		uint16_t m_ConnectionLimit;

	private:
		static SUBSYSTEM_CONSTRUCTOR s_NetworkSubSystemInterfaceFactory;
	};

	CREATE_FACTORY("NetworkSubSystem", NetworkSubSystemInterface);
}
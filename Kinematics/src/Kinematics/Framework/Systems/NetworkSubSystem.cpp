#include "mtepch.h"
#include "NetworkSubSystem.h"

#include "Kinematics/Network/Socket.h"

namespace Kinematics {

	SUBSYSTEM_CONSTRUCTOR NetworkSubSystemInterface::s_NetworkSubSystemInterfaceFactory = [] {return CreateRef<NetworkSubSystem>(); };


	void NetworkSubSystem::Install()
	{
	}

	void NetworkSubSystem::Uninstall()
	{
	}

	void NetworkSubSystem::Initialize()
	{
	}

	void NetworkSubSystem::Shutdown()
	{
	}

	void NetworkSubSystem::Update(Timestep ts)
	{
		if (m_ServerSocket)
			m_ServerSocket->Update();
	}

	std::vector<std::string> NetworkSubSystem::GetDependencies()
	{
		return std::vector<std::string>();
	}

	void NetworkSubSystem::Listen(uint32_t port)
	{
		m_ServerSocket = CreateScope<ServerSocket>(port);
	}

	void NetworkSubSystem::Connect(const char* ip, uint32_t port)
	{
		m_ClientSocket = CreateScope<ClientSocket>(ip, port);
	}

}
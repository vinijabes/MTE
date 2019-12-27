#include "mtepch.h"
#include "NetworkSubSystem.h"

#include "Kinematics/Network/Socket.h"

#define DEFAULT_PORT 27015

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
		Listen(DEFAULT_PORT);
		m_ClientSocket = CreateScope<ClientSocket>("127.0.0.1", DEFAULT_PORT);

		m_ClientSocket->Emit("teste", v());
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
	}

}
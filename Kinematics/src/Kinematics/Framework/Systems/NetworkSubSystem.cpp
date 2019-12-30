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
		{
			if (m_ServerSocket->m_SocketAPI->Closed())
				m_ServerSocket.reset(nullptr);
			else
				m_ServerSocket->Update(ts);
		}

		if (m_ClientSocket)
		{
			if (m_ClientSocket->m_SocketAPI->Closed())
				m_ClientSocket.reset(nullptr);
			else if (m_ClientSocket->GetState() == SocketState::DISCONNECTED)
				m_ClientSocket->SetState(SocketState::RECONNECTING);
			else
				m_ClientSocket->Update(ts);
		}
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

		auto message = ConnectionMessage();
		m_ClientSocket->OnMessage(message);
	}

}
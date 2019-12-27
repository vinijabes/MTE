#include "mtepch.h"
#include "Socket.h"

namespace Kinematics {

	Socket::Socket()
	{
		m_SocketAPI = std::move(s_Factory());
	}

	Socket::Socket(Scope<SocketAPI>& api)
	{
		m_SocketAPI = std::move(api);
	}

	void Socket::Receive()
	{
		m_SocketAPI->Receive();
	}

	ServerSocket::ServerSocket(uint32_t port)
	{
		Listen(port);
		m_Mode = SERVER_MODE;
	}


	void ServerSocket::Update()
	{
		Accept();

		for (auto client : m_SocketAPI->GetClients())
			client->Update();
	}

	void ServerSocket::Close()
	{
		m_SocketAPI->ServerClose();
	}

	void ClientSocket::Update()
	{
		Receive();
	}
}
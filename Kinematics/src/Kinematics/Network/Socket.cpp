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
		auto message = m_SocketAPI->Receive();
		if (message){
			OnMessage(*message);
		}
	}

	ServerSocket::ServerSocket(uint32_t port)
	{
		Listen(port);
		m_Mode = SERVER_MODE;
	}


	void ServerSocket::Update()
	{
		Accept();

		auto begin = m_SocketAPI->GetClients().begin();
		auto end = m_SocketAPI->GetClients().end();
		while(begin != end)
		{
			if (!(*begin)->Closed())
			{
				(*begin)->Update();
				++begin;
			}
			else
			{
				(*begin)->OnMessage(DisconnectionMessage());
				m_SocketAPI->GetClients().erase(begin++);
			}
		}
	}

	void ServerSocket::Close()
	{
		m_SocketAPI->ServerClose();
		auto begin = m_SocketAPI->GetClients().begin();
		auto end = m_SocketAPI->GetClients().end();
		while (begin != end)
		{
			(*begin)->Close();
			(*begin++)->OnMessage(DisconnectionMessage());
		}
	}

	void ClientSocket::Update()
	{
		Receive();
	}
}
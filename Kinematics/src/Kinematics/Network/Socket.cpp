#include "mtepch.h"
#include "Socket.h"

namespace Kinematics {

	Socket::Socket()
	{
		m_SocketAPI = std::move(s_Factory());
		m_SocketAPI->SetKinematicsSocket(this);
	}

	Socket::Socket(Scope<SocketAPI>& api)
	{
		m_SocketAPI = std::move(api);
		m_SocketAPI->SetKinematicsSocket(this);
	}

	void Socket::Receive()
	{
		auto message = m_SocketAPI->Receive();
		if (message) {
			OnMessage(*message);
		}
	}

	ServerSocket::ServerSocket(uint32_t port)
	{
		Listen(port);
		m_Mode = SERVER_MODE;
		m_State = LISTENING;	
	}


	void ServerSocket::Update(Timestep ts)
	{
		Accept();

		auto begin = m_SocketAPI->GetClients().begin();
		auto end = m_SocketAPI->GetClients().end();
		while (begin != end)
		{
			if (!(*begin)->Closed() || !(*begin)->GetState() == SocketState::DISCONNECTED)
			{
				(*begin)->Update(ts);
				++begin;
			}
			else
			{
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
			(*begin++)->Close();
		}
	}

	void ServerSocket::Emit(std::string type, NetworkMessage& message)
	{
		Broadcast(nullptr, type, message);
	}

	void ServerSocket::EmitTo(std::string type, NetworkMessage& message, uint32_t room)
	{
		BroadcastTo(nullptr, type, message, room);
	}

	void ServerSocket::Broadcast(const Ref<ConnectionSocket>& client, std::string type, NetworkMessage& message)
	{
		auto begin = m_SocketAPI->GetClients().begin();
		auto end = m_SocketAPI->GetClients().end();
		while (begin != end)
		{
			if (*begin != client)
				(*begin++)->Emit(type, message);
		}
	}

	void ServerSocket::BroadcastTo(const Ref<ConnectionSocket>& client, std::string type, NetworkMessage& message, uint32_t room)
	{
		if (m_RoomList.find(room) != m_RoomList.end())
		{
			auto begin = m_RoomList[room].begin();
			auto end = m_RoomList[room].end();
			while (begin != end)
			{
				if (*begin != client)
					(*begin++)->Emit(type, message);
			}
		}
	}

	void ServerSocket::Join(uint32_t group, const Ref<ConnectionSocket>& client)
	{
		client->m_Groups.push_back(group);
		m_RoomList[group].push_back(client);
	}

	void ServerSocket::Leave(uint32_t group, const Ref<ConnectionSocket>& client)
	{
		if (m_RoomList.find(group) != m_RoomList.end()) {
			m_RoomList[group].remove(client);
			client->m_Groups.remove(group);
		}
	}

	void ServerSocket::OnDisconnection(const Ref<ConnectionSocket>& client)
	{
		for (auto group : client->m_Groups) 
		{
			m_RoomList[group].remove(client);
			if (m_RoomList[group].size() == 0) 
			{
				m_RoomList.erase(group);
			}
		}
	}

	void ClientSocket::Update(Timestep ts)
	{
		if (m_State == CONNECTING || m_State == RECONNECTING) 
		{
			m_Timeout += ts;
			if (m_Timeout > 4.0f) 
			{
				if (Connect(m_Addr, m_Port) == KINEMATICS_SOCKET_WOULD_BLOCK) 
				{
					SetState(SocketState::CONNECTED);
				}
				else
				{
					m_Timeout = 0.0f;
					if (++m_ConnectionTries == 5) 
					{
						Close();
					}
				}
			}
		}
		else if (m_State == CONNECTED)
		{
			Receive();
		}
	}

	void ConnectionSocket::Update(Timestep step)
	{
		Receive();
	}
	void ConnectionSocket::Close()
	{
		m_SocketAPI->ClientClose();
		SetState(SocketState::CLOSED);		
	}
}
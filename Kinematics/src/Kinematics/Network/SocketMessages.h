#pragma once

#include "NetworkMessage.h"
#include "Kinematics/Network/Socket.h"

namespace Kinematics {
	class ConnectionMessage : public NetworkMessage
	{
	public:
		ConnectionMessage() : NetworkMessage("connection"), m_Socket(nullptr)
		{

		}		

		ConnectionMessage(Ref<ConnectionSocket> socket) : NetworkMessage("connection"), m_Socket(socket)
		{

		}

		Ref<ConnectionSocket> GetSocket() { return m_Socket; }

		NETWORK_MESSAGE_TYPE(ConnectionMessage);
	private:
		Ref<ConnectionSocket> m_Socket;
	};

	class DisconnectionMessage : public NetworkMessage
	{
	public:
		DisconnectionMessage() : NetworkMessage("disconnection")
		{

		}

		NETWORK_MESSAGE_TYPE(DisconnectionMessage);
	};

	class PingMessage : public NetworkMessage
	{
		NETWORK_MESSAGE_TYPE(PingMessage);
	};
}
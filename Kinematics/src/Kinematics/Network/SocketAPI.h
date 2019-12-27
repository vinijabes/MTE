#pragma once

#include <string>
#include "NetworkMessage.h"
#include "Kinematics/Network/Packet.h"

namespace Kinematics {

	class ClientSocket;

	class SocketAPI
	{
	public:
		virtual void Connect(std::string addr, unsigned int port) = 0;
		virtual void Listen(unsigned int port) = 0;
		virtual void ClientClose() = 0;
		virtual void ServerClose() = 0;
		virtual Ref<ClientSocket> Accept() = 0;

		virtual void Emit(std::string type, NetworkMessage& message) = 0;
		virtual Ref<NetworkMessage> Receive() = 0;

		virtual bool Closed() = 0;

		std::list<Ref<ClientSocket>> & GetClients() { return m_Clients; }
	protected:
		std::list<Ref<ClientSocket>> m_Clients;
	};
}
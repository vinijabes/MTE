#pragma once

#include <string>
#include <list>

#include "NetworkMessage.h"
#include "Kinematics/Network/Packet.h"

namespace Kinematics {

	class Socket;
	class ConnectionSocket;

	class SocketAPI
	{
	public:
		virtual int Connect(std::string addr, unsigned int port) = 0;
		virtual int Listen(unsigned int port) = 0;
		virtual void ClientClose() = 0;
		virtual void ServerClose() = 0;
		virtual Ref<ConnectionSocket> Accept() = 0;

		virtual void Emit(std::string type, NetworkMessage& message) = 0;
		virtual Ref<NetworkMessage> Receive() = 0;

		virtual bool Closed() = 0;

		void SetKinematicsSocket(Socket* socket) { m_KinematicsSocket = socket; };

		std::list<Ref<ConnectionSocket>> & GetClients() { return m_Clients; }
	protected:
		std::list<Ref<ConnectionSocket>> m_Clients;
		Socket* m_KinematicsSocket;		
	};
}
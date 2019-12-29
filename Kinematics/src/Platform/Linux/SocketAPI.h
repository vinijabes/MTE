#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h> 


#include "Kinematics/Network/SocketAPI.h"

namespace Kinematics {
	
	typedef int SOCKET;
	class LinuxSocketAPI : public SocketAPI
	{
	public:
		LinuxSocketAPI() : m_Socket(-1), m_Closed(false) {};
		LinuxSocketAPI(SOCKET socket) : m_Socket(socket), m_Closed(false) {}

		virtual void Connect(std::string addr, unsigned int port) override;
		virtual void Listen(uint32_t port) override;
		virtual void ClientClose() override;
		virtual void ServerClose() override;
		virtual Ref<ClientSocket> Accept() override;

		virtual void Emit(std::string type, NetworkMessage& message) override;
		virtual Ref<NetworkMessage> Receive() override;

		virtual bool Closed() override;
	private:
		SOCKET m_Socket;
		bool m_Closed;
	};

}

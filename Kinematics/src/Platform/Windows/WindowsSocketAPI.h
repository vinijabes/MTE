#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>

#include "Kinematics/Network/SocketAPI.h"

namespace Kinematics {
	
	class WindowsSocketAPI : public SocketAPI
	{
	public:
		WindowsSocketAPI() : m_Socket(INVALID_SOCKET) {};
		WindowsSocketAPI(SOCKET socket) : m_Socket(socket) {}

		virtual void Connect(std::string addr, unsigned int port) override;
		virtual void Listen(uint32_t port) override;
		virtual void ClientClose() override;
		virtual void ServerClose() override;
		virtual Ref<ClientSocket> Accept() override;

		virtual void Emit(std::string type, NetworkMessage& message) override;
		virtual void Receive() override;
	private:
		SOCKET m_Socket;
	};

}

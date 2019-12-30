#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>

#include "Kinematics/Network/SocketAPI.h"

namespace Kinematics {
	
	class WindowsSocketAPI : public SocketAPI
	{
	public:
		WindowsSocketAPI() : m_Socket(INVALID_SOCKET), m_Closed(false) {};
		WindowsSocketAPI(SOCKET socket) : m_Socket(socket), m_Closed(false) {}

		virtual int Connect(std::string addr, unsigned int port) override;
		virtual int Listen(uint32_t port) override;
		virtual void ClientClose() override;
		virtual void ServerClose() override;
		virtual Ref<ConnectionSocket> Accept() override;

		virtual void Emit(std::string type, NetworkMessage& message) override;
		virtual Ref<NetworkMessage> Receive() override;

		virtual bool Closed() override;
	private:
		SOCKET m_Socket;
		bool m_Closed;
		TIMEVAL m_Timeout;

		FD_SET m_WriteSet;
		FD_SET m_ReadSet;
		FD_SET m_ExceptionSet;
	};

}

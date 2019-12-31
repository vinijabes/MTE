#include "mtepch.h"
#include "SocketAPI.h"

#include "Kinematics/Network/Socket.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Kinematics {
	std::function<Scope<SocketAPI>()> Socket::s_Factory = [] { return CreateScope<WindowsSocketAPI>(); };

	int WindowsSocketAPI::Connect(std::string addr, unsigned int port)
	{
		m_Timeout.tv_sec = 0;
		m_Timeout.tv_usec = 200000;

		WSADATA wsaData;
		int iResult;
		u_long iMode = 1;

		struct sockaddr_in my_addr, my_addr1;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
			printf("Error at WSAStartup()\n");

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
			return KINEMATICS_SOCKET_ERROR;
		}

		iResult = ioctlsocket(m_Socket, FIONBIO, &iMode);
		if (iResult != NO_ERROR)
			printf("ioctlsocket failed with error: %ld\n", iResult);

		my_addr.sin_family = AF_INET;
		my_addr.sin_addr.s_addr = INADDR_ANY;
		my_addr.sin_port = htons(port);

		my_addr.sin_addr.s_addr = inet_addr(addr.c_str());

		bind(m_Socket, (struct sockaddr*) & my_addr1, sizeof(struct sockaddr_in));
		socklen_t addr_size = sizeof my_addr;

		iResult = connect(m_Socket, (struct sockaddr*) & my_addr, sizeof my_addr);

		if (iResult == SOCKET_ERROR)
		{
			int ierr = WSAGetLastError();
			if (ierr != WSAEWOULDBLOCK)
			{
				closesocket(m_Socket);
				WSACleanup();
				m_Socket = INVALID_SOCKET;
				return KINEMATICS_SOCKET_ERROR;
			}

			FD_ZERO(&m_WriteSet);
			FD_ZERO(&m_ReadSet);
			FD_ZERO(&m_ExceptionSet);

			FD_SET(m_Socket, &m_WriteSet);
			FD_SET(m_Socket, &m_ExceptionSet);

			int ret = select(0, NULL, &m_WriteSet, &m_ExceptionSet, &m_Timeout);
			if (ret <= 0)
			{
				closesocket(m_Socket);
				WSACleanup();
				m_Socket = INVALID_SOCKET;
				return KINEMATICS_SOCKET_ERROR;
			}

			if (FD_ISSET(m_Socket, &m_ExceptionSet))
			{
				closesocket(m_Socket);
				WSACleanup();
				m_Socket = INVALID_SOCKET;
				return KINEMATICS_SOCKET_ERROR;
			}
		}

		return KINEMATICS_SOCKET_WOULD_BLOCK;
	}

	int WindowsSocketAPI::Listen(uint32_t port)
	{
		WSADATA wsaData;
		int iResult;
		u_long iMode = 1;

		SOCKADDR_IN serverAddr;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
			printf("Error at WSAStartup()\n");

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
			return KINEMATICS_SOCKET_INVALID_SOCKET;
		}

		iResult = ioctlsocket(m_Socket, FIONBIO, &iMode);
		if (iResult != NO_ERROR)
			printf("ioctlsocket failed with error: %ld\n", iResult);

		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);

		bind(m_Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
		listen(m_Socket, 0);

		KINEMATICS_CORE_INFO("Listening for incoming connections on port: {}", port);
	}

	void WindowsSocketAPI::ServerClose()
	{
		KINEMATICS_CORE_INFO("Closing server socket");

		closesocket(m_Socket);
		WSACleanup();
		m_Closed = true;
	}

	void WindowsSocketAPI::ClientClose()
	{
		KINEMATICS_CORE_INFO("Closing client socket");
		closesocket(m_Socket);
		m_Closed = true;
	}

	Ref<ConnectionSocket> WindowsSocketAPI::Accept()
	{
		if (m_Socket == INVALID_SOCKET) return nullptr;

		SOCKET AcceptSocket = accept(m_Socket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK)
			{
				return nullptr;
			}
			else
			{
				wprintf(L"accept failed with error: %ld\n", ierr);
				closesocket(m_Socket);
				WSACleanup();

				m_Socket = INVALID_SOCKET;
				return nullptr;
			}
		}

		Ref<ConnectionSocket> client = CreateRef<ConnectionSocket>(CreateScope<WindowsSocketAPI>(AcceptSocket));
		m_Clients.push_back(client);

		return client;
	}

	void WindowsSocketAPI::Emit(std::string type, NetworkMessage& message)
	{
		OPacket packet;
		packet& type;
		packet& message;
		packet.MakeHeader();

		int iResult = send(m_Socket, packet, packet.GetSize(), 0);
		if (iResult == SOCKET_ERROR)
		{
			wprintf(L"send failed with error: %d\n", WSAGetLastError());
		}
	}

#define DEFAULT_BUFLEN 512

	Ref<NetworkMessage> WindowsSocketAPI::Receive()
	{
		recvbuflen = DEFAULT_BUFLEN;

		int iResult = recv(m_Socket, recvbuf, sizeof(size_t), 0);
		if (iResult > 0)
		{
			IPacket packet(recvbuf, iResult);
			size_t count;
			Ref<NetworkMessage> message;
			while ((count = packet.Remaining()))
			{
				iResult = recv(m_Socket, recvbuf, count, 0);
				if (iResult > 0)
				{
					packet.Add(recvbuf, iResult);
				}
				else if (iResult == 0)
				{
					m_Closed = true;
					return nullptr;
				}
				else
				{
					int ierr = WSAGetLastError();
					if (ierr == WSAEWOULDBLOCK) return nullptr;
					if (ierr == WSAECONNRESET)
					{
						m_KinematicsSocket->SetState(SocketState::DISCONNECTED);
						//m_Closed = true;
						return nullptr;
					}

					printf("recv failed: %d\n", ierr);
				}
			}
			if (packet)
			{
				std::string type;
				packet& type;
				message = FactoryManager::GetInstance()->CreateMessage(type);
				packet&* message;

				return message;
			}
		}
		else if (iResult == 0)
		{
			m_Closed = true;
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) return nullptr;
			if (ierr == WSAECONNRESET)
			{
				m_KinematicsSocket->SetState(SocketState::DISCONNECTED);
				//m_Closed = true;
				return nullptr;
			}

			printf("recv failed: %d\n", ierr);
		}

		return nullptr;
	}
	bool WindowsSocketAPI::Closed()
	{
		return m_Closed;
	}
}
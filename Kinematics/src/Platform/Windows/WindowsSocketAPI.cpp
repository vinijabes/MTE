#include "mtepch.h"
#include "WindowsSocketAPI.h"

#include "Kinematics/Network/Socket.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Kinematics {
	std::function<Scope<SocketAPI>()> Socket::s_Factory = [] { return CreateScope<WindowsSocketAPI>(); };

	void WindowsSocketAPI::Connect(std::string addr, unsigned int port)
	{
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
			return;
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

		if (connect(m_Socket, (struct sockaddr*) & my_addr, sizeof my_addr) == 0)
			printf("Client Connected\n");
	}

	void WindowsSocketAPI::Listen(uint32_t port)
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
			return;
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
	}

	void WindowsSocketAPI::ClientClose()
	{
		KINEMATICS_CORE_INFO("Closing client socket");
		closesocket(m_Socket);
	}

	Ref<ClientSocket> WindowsSocketAPI::Accept()
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
		else
		{
			wprintf(L"Client connected.\n");
		}

		Ref<ClientSocket> client = CreateRef<ClientSocket>(CreateScope<WindowsSocketAPI>(AcceptSocket));
		m_Clients.push_back(client);

		return client;
	}

	void WindowsSocketAPI::Emit(std::string type, NetworkMessage& message)
	{
		int iResult = send(m_Socket, message, message.size(), 0);
		if (iResult == SOCKET_ERROR)
		{
			wprintf(L"send failed with error: %d\n", WSAGetLastError());
		}
	}

#define DEFAULT_BUFLEN 512

	void WindowsSocketAPI::Receive()
	{
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		int iResult = recv(m_Socket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			recvbuf[iResult] = '\0';
			printf("Bytes received: %s\n", recvbuf);
			v teste = v(recvbuf);
			std::cout << sizeof(NetworkMessage) << " " << sizeof(v) << " " << iResult << " " << teste.x << std::endl;
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) return;

			printf("recv failed: %d\n", ierr);
		}
	}
}
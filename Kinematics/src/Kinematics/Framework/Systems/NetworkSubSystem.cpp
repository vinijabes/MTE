#include "mtepch.h"
#include "NetworkSubSystem.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

namespace Kinematics {

	SUBSYSTEM_CONSTRUCTOR NetworkSubSystemInterface::s_NetworkSubSystemInterfaceFactory = [] {return CreateRef<NetworkSubSystem>(); };


	void NetworkSubSystem::Install()
	{
	}

	void NetworkSubSystem::Uninstall()
	{
	}

	void NetworkSubSystem::Initialize()
	{
		Listen(0);
	}

	void NetworkSubSystem::Shutdown()
	{
		closesocket(m_ClientSocket);
		WSACleanup();
	}

	void NetworkSubSystem::Update(Timestep ts)
	{
	}

	std::vector<std::string> NetworkSubSystem::GetDependencies()
	{
		return std::vector<std::string>();
	}

	void NetworkSubSystem::Listen(uint32_t port)
	{
		WSADATA wsaData;
		int iResult;

		KinematicsSocket ListenSocket = KINEMATICS_INVALID_SOCKET;
		m_ServerSocket = KINEMATICS_INVALID_SOCKET;

		struct addrinfo* result = NULL;
		struct addrinfo  hints;

		int iSendResult;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		char* errorMessage = (char*)malloc(40 * sizeof(char));

		sprintf(errorMessage, "WSAStartup failed with error : %d\n", iResult);
		KINEMATICS_ASSERT(iResult == 0, errorMessage);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

		sprintf(errorMessage, "getaddrinfo failed with error: %d\n", iResult);
		KINEMATICS_ASSERT(iResult == 0, errorMessage);

		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		sprintf(errorMessage, "socket failed with error: %ld\n", WSAGetLastError());
		KINEMATICS_ASSERT(ListenSocket != INVALID_SOCKET, errorMessage);


		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

		sprintf(errorMessage, "bind failed with error: %d\n", WSAGetLastError());
		KINEMATICS_ASSERT(iResult != SOCKET_ERROR, errorMessage);

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);

		m_ClientSocket = accept(ListenSocket, NULL, NULL);
	}

	void NetworkSubSystem::Connect(const char* ip, uint32_t port)
	{
	}

}
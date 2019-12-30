#include "mtepch.h"
#include "SocketAPI.h"

#include "Kinematics/Network/Socket.h"

#include <errno.h>

namespace Kinematics {
	std::function<Scope<SocketAPI>()> Socket::s_Factory = [] { return CreateScope<LinuxSocketAPI>(); };

	int LinuxSocketAPI::Connect(std::string addr, unsigned int port)
	{
		struct sockaddr_in serv_addr;
    	struct hostent *server;

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		int flags = fcntl(m_Socket, F_GETFL);
		fcntl(m_Socket, F_SETFL, flags | O_NONBLOCK);

		 server = gethostbyname(addr.c_str());

		if (server == NULL) 
		{
			KINEMATICS_ERROR("No such host with address {}", addr);
    	}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, 
			  (char *)&serv_addr.sin_addr.s_addr,
			   server->h_length);
		serv_addr.sin_port = htons(port);

		if (connect(m_Socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
			KINEMATICS_ERROR("Connecting error");
		else
		{
			return KINEMATICS_SOCKET_WOULD_BLOCK;
		}
	}

	int LinuxSocketAPI::Listen(uint32_t port)
	{
		struct sockaddr_in serv_addr, cli_addr;
		int flags;

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		flags = fcntl(m_Socket, F_GETFL);
		fcntl(m_Socket, F_SETFL, flags | O_NONBLOCK);

		if(m_Socket < 0)
			KINEMATICS_CORE_ERROR("ERROR opening socket");

		bzero((char *) &serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		if (bind(m_Socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
			KINEMATICS_CORE_ERROR("ERROR on binding");
		
		listen(m_Socket, 0);

		KINEMATICS_CORE_INFO("Listening for incoming connections on port: {}", port);		
	}

	void LinuxSocketAPI::ServerClose()
	{
		KINEMATICS_CORE_INFO("Closing server socket");
		close(m_Socket);
		m_Closed = true;
	}

	void LinuxSocketAPI::ClientClose()
	{
		KINEMATICS_CORE_INFO("Closing client socket");
		close(m_Socket);
		m_Closed = true;
	}

	Ref<ConnectionSocket> LinuxSocketAPI::Accept()
	{
		socklen_t clilen;
		struct sockaddr_in cli_addr;
		clilen = sizeof(cli_addr);
     	int AcceptSocket = accept(m_Socket, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

		if (AcceptSocket == -1){
			if(errno == EWOULDBLOCK)
				return nullptr;
			else
			{
				close(m_Socket);
				m_Socket = -1;
				return nullptr;
			}
		} 

		int flags = fcntl(AcceptSocket, F_GETFL);
		fcntl(AcceptSocket, F_SETFL, flags | O_NONBLOCK);

		Ref<ConnectionSocket> client = CreateRef<ConnectionSocket>(CreateScope<LinuxSocketAPI>(AcceptSocket));
		m_Clients.push_back(client);

		return client;
	}

	void LinuxSocketAPI::Emit(std::string type, NetworkMessage& message)
	{
		OPacket packet;
		packet& type;
		packet& message;
		packet.MakeHeader();

		int iResult = write(m_Socket, packet, packet.GetSize());
		if (iResult < 0)
		{
			KINEMATICS_CORE_ERROR("ERROR writing to socket");
		}
	}

#define DEFAULT_BUFLEN 512

	Ref<NetworkMessage> LinuxSocketAPI::Receive()
	{
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		int iResult = read(m_Socket, recvbuf, recvbuflen);
		if(iResult > 0){
			IPacket packet(recvbuf, iResult);
			size_t count;
			Ref<NetworkMessage> message;

			while ((count = packet.Remaining()))
			{
				iResult = read(m_Socket, recvbuf, recvbuflen);
				if (iResult > 0)
				{
					packet.Add(recvbuf, iResult);
				}
				else if(iResult == 0)
				{
					m_Closed = true;
					return nullptr;
				}
				else
				{
					if(errno == EWOULDBLOCK)
						return nullptr;
				}
			}

			if (packet)
			{
				std::string type;
				packet& type;
				message = FactoryManager::GetInstance()->CreateMessage(type);
				packet&* message;

				KINEMATICS_INFO("Receiving packet of type: {}", type);
				return message;
			}
		}
		else if(iResult == 0)
		{
			m_Closed = true;
			return nullptr;
		}
		else
		{
			if(errno == EWOULDBLOCK)
				return nullptr;
		}

		return nullptr;
	}
	bool LinuxSocketAPI::Closed()
	{
		return m_Closed;
	}
}
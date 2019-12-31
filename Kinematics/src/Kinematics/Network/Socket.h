#pragma once

#include "Kinematics/Core/Core.h"

#include "SocketAPI.h"
#include "NetworkMessage.h"
#include "SocketMessages.h"

#include <unordered_map>

namespace Kinematics {

#define KINEMATICS_SOCKET_INVALID_SOCKET	-1
#define KINEMATICS_SOCKET_WOULD_BLOCK		-2
#define KINEMATICS_SOCKET_CONNECTING		-3
#define KINEMATICS_SOCKET_ERROR				-4

	enum SocketMode
	{
		CLIENT_MODE,
		SERVER_MODE
	};

	enum SocketState
	{
		OPEN,
		CONNECTING,
		CONNECTED,
		LISTENING,
		DISCONNECTED,
		RECONNECTING,
		CLOSED
	};

	typedef std::function<void(NetworkMessage&)> SocketCallback;
	class Socket
	{
	public:
		Socket();
		Socket(Scope<SocketAPI>& api);

		virtual ~Socket() {};

		void On(std::string type, SocketCallback cb) { m_CallbackMap[type] = cb; }
		virtual void Emit(std::string type, NetworkMessage& message) { m_SocketAPI->Emit(type, message); }

		virtual void Update(Timestep step) = 0;
		virtual void Close() = 0;

		void Receive();
		uint32_t GetID() { return m_SocketAPI->GetID(); }

		SocketMode GetMode() { return m_Mode; }
		SocketState GetState() { return m_State; }
		void SetState(SocketState state)
		{
			if (state == m_State) return;
			m_State = state;

			if (m_State == SocketState::CONNECTED)
			{
				auto message = ConnectionMessage();
				OnMessage(message);
			}

			if (m_State == SocketState::CLOSED)
			{
				auto message = DisconnectionMessage();
				OnMessage(message);
			}
		}

	protected:

		int Connect(std::string addr, uint32_t port) { return m_SocketAPI->Connect(addr, port); }
		void Listen(uint32_t port) { m_SocketAPI->Listen(port); }

		void OnMessage(NetworkMessage& message)
		{
			if (m_CallbackMap.find(message.GetType()) != m_CallbackMap.end())
				m_CallbackMap[message.GetType()](message);
		}

		friend class ClientSocket;
		friend class ServerSocket;
		friend class NetworkSubSystem;

	protected:
		static std::function<Scope<SocketAPI>()> s_Factory;
		Scope<SocketAPI> m_SocketAPI;

		SocketMode m_Mode;
		SocketState m_State;
		std::unordered_map<std::string, SocketCallback> m_CallbackMap;
	};

	class ClientSocket;
	class ServerSocket;
	class ConnectionSocket;

	class ServerSocket : public Socket
	{
	public:
		ServerSocket(uint32_t port);
		~ServerSocket() { Close(); }

		void Accept();

		virtual void Update(Timestep ts) override;
		virtual void Close() override;

		virtual void Emit(std::string type, NetworkMessage& message) override;
		void EmitTo(std::string type, NetworkMessage& message, uint32_t room);

		void Broadcast(ConnectionSocket* client, std::string type, NetworkMessage& message);
		void BroadcastTo(ConnectionSocket* client, std::string type, NetworkMessage& message, uint32_t room);

		void Join(uint32_t group, const Ref<ConnectionSocket>& client);
		void Leave(uint32_t group, const Ref<ConnectionSocket>& client);
	protected:
		void OnDisconnection(const Ref<ConnectionSocket>& client);

	private:
		std::unordered_map <uint32_t, std::list<Ref<ConnectionSocket>>> m_RoomList;
	};

	class ConnectionSocket : public Socket
	{
	public:
		friend class ServerSocket;

		ConnectionSocket(Scope<SocketAPI> api)
			: Socket(api)
		{
		}

		~ConnectionSocket()
		{
			Close();
		}

		void SetServer(ServerSocket* server) { m_Server = server; }
		ServerSocket* GetServer() { return m_Server; }

		virtual void Update(Timestep step) override;
		virtual void Close() override;

		void Broadcast(std::string type, NetworkMessage& message) { m_Server->Broadcast(this, type, message); };
		void BroadcastTo(std::string type, NetworkMessage& message, uint32_t room) { m_Server->BroadcastTo(this, type, message, room); };

		bool Closed()
		{
			return m_SocketAPI->Closed();
		}

	protected:
		std::list<uint32_t> m_Groups;
		ServerSocket* m_Server;
	};

	class ClientSocket : public Socket
	{
	public:

		ClientSocket(std::string addr, uint32_t port)
			: m_Timeout(0.0f), m_Addr(addr), m_Port(port), m_ConnectionTries(0)
		{
			m_Mode = CLIENT_MODE;
			m_State = CONNECTING;
		}

		~ClientSocket() { if (!m_SocketAPI->Closed()) Close(); }

		void Update(Timestep ts) override;

		void Close() override
		{
			m_SocketAPI->ServerClose();
			SetState(SocketState::CLOSED);
		}

		bool Closed()
		{
			return m_SocketAPI->Closed();
		}
	protected:
		std::string m_Addr;
		uint32_t m_Port;

		float m_Timeout;
		uint8_t m_ConnectionTries;
	};
}
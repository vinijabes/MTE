#pragma once

#include "Kinematics/Core/Core.h"

#include "SocketAPI.h"
#include "NetworkMessage.h"
#include "SocketMessages.h"


namespace Kinematics {

	enum SocketMode
	{
		CLIENT_MODE,
		SERVER_MODE
	};

	typedef std::function<void(NetworkMessage&)> SocketCallback;
	class Socket
	{
	public:
		Socket();
		Socket(Scope<SocketAPI>& api);

		virtual ~Socket() {};

		void On(std::string type, SocketCallback cb) { m_CallbackMap[type] = cb; }
		void Emit(std::string type, NetworkMessage& message) { m_SocketAPI->Emit(type, message); }

		virtual void Update() = 0;
		virtual void Close() = 0;

		void Receive();

		SocketMode GetMode() { return m_Mode; }

	protected:

		void Connect(std::string addr, uint32_t port) { m_SocketAPI->Connect(addr, port); }
		void Listen(uint32_t port) { m_SocketAPI->Listen(port); }

		void OnMessage(NetworkMessage& message)
		{
			if(m_CallbackMap.find(message.GetType()) != m_CallbackMap.end())
				m_CallbackMap[message.GetType()](message);
		}

		friend class ClientSocket;
		friend class ServerSocket;
		friend class NetworkSubSystem;

	protected:
		static std::function<Scope<SocketAPI>()> s_Factory;
		Scope<SocketAPI> m_SocketAPI;

		SocketMode m_Mode;
		std::unordered_map<std::string, SocketCallback> m_CallbackMap;
	};

	class ClientSocket;
	class ServerSocket;

	class ServerSocket : public Socket
	{
	public:
		ServerSocket(uint32_t port);
		~ServerSocket() { Close(); }

		void Accept()
		{
			auto client = m_SocketAPI->Accept();
			if (client)
				OnMessage(ConnectionMessage());
		}

		virtual void Update() override;
		virtual void Close() override;
	};

	class ClientSocket : public Socket
	{
		enum ClientMode
		{
			ACTIVE_MODE, //Client Side
			SERVO_MODE //Server Side
		};
	public:

		ClientSocket(std::string addr, uint32_t port)
			: m_ClientMode(ACTIVE_MODE)
		{
			Connect(addr, port);

			m_Mode = CLIENT_MODE;
		}

		ClientSocket(Scope<SocketAPI> api)
			: Socket(api), m_ClientMode(SERVO_MODE)
		{
		}

		~ClientSocket() { Close(); }

		void Update() override;

		void Close() override
		{
			m_SocketAPI->ClientClose();
			OnMessage(DisconnectionMessage());
		}

		bool Closed()
		{
			return m_SocketAPI->Closed();
		}

	private:
		ClientMode m_ClientMode;
	};
}
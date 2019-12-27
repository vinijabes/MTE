#pragma once

#include "NetworkMessage.h"

namespace Kinematics {
	class ConnectionMessage : public NetworkMessage
	{
	public:
		ConnectionMessage() : NetworkMessage("connection")
		{

		}

		NETWORK_MESSAGE_TYPE(ConnectionMessage);
	};

	class DisconnectionMessage : public NetworkMessage
	{
	public:
		DisconnectionMessage() : NetworkMessage("disconnection")
		{

		}

		NETWORK_MESSAGE_TYPE(DisconnectionMessage);
	};
}
#pragma once
#include "Packet.h"

#include "Kinematics/Framework/Managers/FactoryManager.h"

namespace Kinematics {

#define NETWORK_MESSAGE_TYPE(type) static const char * GetStaticType() { return #type; }

	struct NetworkMessage
	{
	public:
		NetworkMessage(const std::string& type) : m_Type(type) {}

		operator char* () const { return (char*)this; }

		virtual size_t size() { return sizeof(*this); }
		std::string const GetType() const { return m_Type; }

		virtual void Serialize(IPacket& p) {};
		virtual void Serialize(OPacket& p) {};

	private:
		std::string m_Type;
	};
}
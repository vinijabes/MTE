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

	struct v : public NetworkMessage
	{
		v() : NetworkMessage("Test") {}
		v(int x, int y) : NetworkMessage("Test"), x(x), y(y) {}
		v(char* data) : NetworkMessage("Test")
		{
			*this = *((v*)(data));
		}

		virtual size_t size() { return sizeof(*this); }

		void Serialize(IPacket& p) override
		{
			p& x;
			p& y;
		}

		void Serialize(OPacket& p) override
		{
			p& x;
			p& y;
		}

		int x = 2;
		int y = 3;

		NETWORK_MESSAGE_TYPE(v);
	};

	CREATE_MESSAGE_FACTORY("v", v);
}
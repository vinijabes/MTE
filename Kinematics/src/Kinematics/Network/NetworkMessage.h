#pragma once

namespace Kinematics {
	struct NetworkMessage
	{
	public:
		NetworkMessage(const std::string& type) : m_Type(type) {}

		operator char* () const { return (char*)this; }

		virtual size_t size() { return sizeof(*this); }
		std::string const GetType() const { return m_Type; }
	private:
		std::string m_Type;
	};

	struct v : public NetworkMessage
	{
		v() : NetworkMessage("Test") {}
		v(char* data) : NetworkMessage("Test")
		{
			*this = *((v*)(data));
		}

		virtual size_t size() { return sizeof(*this); }

		int x = 2;
		int y = 3;
	};
}
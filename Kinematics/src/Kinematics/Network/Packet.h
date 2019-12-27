#pragma once
#include <iostream>

namespace Kinematics {

	typedef char* BYTEARRAY;

	class Packet
	{
	public:
		template <typename T>

		friend std::ostream& operator<<(std::ostream& output, T element)
		{
			this << element;

			return output;
		}

		friend std::ostream& operator<<(std::ostream& output, int element)
		{
			BYTEARRAY bytes = (BYTEARRAY)element;
			m_Data += std::string(bytes[0], bytes[sizeof(element) - 1]);

			return output;
		}

		friend std::ostream& operator<<(std::ostream& output, char element)
		{
			BYTEARRAY bytes = (BYTEARRAY)element;
			m_Data += std::string(bytes[0], bytes[sizeof(element) - 1]);

			return output;
		}

		friend std::ostream& operator<<(std::ostream& output, float element)
		{
			BYTEARRAY bytes = (BYTEARRAY)element;
			m_Data += std::string(bytes[0], bytes[sizeof(element) - 1]);

			return output;
		}

		friend std::ostream& operator<<(std::ostream& output, double element)
		{
			BYTEARRAY bytes = (BYTEARRAY)element;
			m_Data += std::string(bytes[0], bytes[sizeof(element) - 1]);

			return output;
		}
	private:
		std::string m_Data;
		size_t m_Pos;
	};
}
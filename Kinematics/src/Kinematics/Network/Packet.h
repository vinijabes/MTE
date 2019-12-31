#pragma once
#include <iostream>
#include <type_traits> // To use 'std::integral_constant'.

namespace Kinematics {
	class Packet;

	template<class T>
	using HasSerializable = decltype(std::declval<T&>().Serialize(std::declval<Packet&>()));

	class Serializable
	{
	public:
		virtual void Serialize() = 0;
	};

	class Packet
	{
	public:
		operator char* const ()
		{
			return m_Data;
		}

		virtual size_t GetSize() const { return m_Size; };
	protected:
		char* m_Data;
		size_t m_Pos;
		size_t m_Size;
	};

	class OPacket : public Packet
	{
	public:
		OPacket(size_t size = 64)
		{
			m_Data = (char*)malloc(size);

			if (!m_Data) KINEMATICS_TRACE("INVALID MEMORY");

			m_Pos = sizeof(size_t);
			m_Size = size;
		}

		~OPacket()
		{
			free(m_Data);
		}

		template<typename T>
		auto serialize(T& element, int)
			-> decltype(std::declval<T>().Serialize(std::declval<OPacket&>()))
		{
			element.Serialize(*this);
			return;
		}

		template<typename T>
		void serialize(T& element, ...)
		{
			if (m_Size >= m_Pos + sizeof(T))
			{
				m_Data = (char*)realloc(m_Data, (m_Pos + sizeof(T)) * 2);
			}

			if (m_Data)
			{
				memcpy(m_Data + m_Pos, &element, sizeof(T));
				m_Pos += sizeof(T);
				m_Size = (m_Pos + sizeof(T)) * 2;
			}
		}

		template<typename T>
		OPacket& operator&(T& element)
		{
			serialize(element, 0);
			return *this;
		}

		OPacket& operator&(std::string& element)
		{
			size_t size = element.size();
			const char* cstr = element.c_str();
			serialize(size, 0);

			if (m_Size >= m_Pos + size * sizeof(char))
			{
				m_Data = (char*)realloc(m_Data, (m_Pos + size * sizeof(char)) * 2);
			}

			if (m_Data)
			{
				memcpy(m_Data + m_Pos, cstr, size);
				m_Pos += size;
				m_Size = (m_Pos + sizeof(char)) * 2;
			}

			return *this;
		}

		virtual size_t GetSize() const { return m_Pos; };

		void MakeHeader()
		{
			memcpy(m_Data, &m_Pos, sizeof(size_t));
		}

		friend class IPacket;
	};

	class IPacket : public Packet
	{
	public:
		IPacket(char* data, size_t len)
		{
			if (len < sizeof(size_t))
			{
				m_Pos = m_PacketLen = m_Size = 0;
				return;
			}

			size_t size;
			memcpy(&size, data, sizeof(size_t));
			m_Data = (char*)malloc(size);
			if (m_Data != nullptr)
			{
				memcpy(m_Data, data, len);
				m_Size = len;
				m_Pos = sizeof(size_t);

				m_PacketLen = size;
			}
		}

		IPacket(OPacket& i)
		{
			m_Data = (char*)malloc(i.m_Size);
			if (m_Data != nullptr)
			{
				memcpy(m_Data, i.m_Data, i.m_Size);
				m_Size = i.m_Size;
				m_Pos = 0;
			}
		}

		~IPacket()
		{
			if (m_Data) 
			{
				free(m_Data);
			}
		}

		void Add(char* data, size_t len)
		{
			if (m_Data != nullptr)
			{
				memcpy(m_Data + m_Size, data, len);
				m_Size += len;
			}
		}

		template<typename T>
		auto serialize(T& element, int)
			-> decltype(std::declval<T>().Serialize(std::declval<IPacket&>()))
		{
			element.Serialize(*this);
			return;
		}

		template<typename T>
		void serialize(T& element, ...)
		{
			if (m_Pos + sizeof(element) <= m_Size)
			{
				memcpy(&element, m_Data + m_Pos, sizeof(T));
				m_Pos += sizeof(element);
			}
		}

		template<typename T>
		IPacket* operator&(T& element)
		{
			serialize(element, 0);
			return this;
		}

		IPacket* operator&(std::string& element)
		{
			if (m_Pos + sizeof(size_t) <= m_Size)
			{
				size_t size;
				serialize(size);
				if (m_Pos + size <= m_Size)
				{
					char* data = (char*)malloc(size);
					memcpy(data, m_Data + m_Pos, size);
					element = std::string(data, data + size);

					m_Pos += size;
				}
			}
			return this;
		}

		operator bool() const {
			return m_PacketLen != m_Pos;
		}

		size_t Remaining()
		{
			if (m_Size > m_PacketLen) return 0;
			return m_PacketLen - m_Size;
		}

	private:
		size_t m_PacketLen;
	};
}
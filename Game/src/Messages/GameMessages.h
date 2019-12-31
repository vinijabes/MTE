#pragma once

#include <Kinematics.h>
#include <vector>

#include "../Creature.h"

namespace Game
{
	class InputMessage : public Kinematics::NetworkMessage
	{
	public:
		InputMessage() : Kinematics::NetworkMessage("input"), m_KeyValue(0)
		{
		}

		InputMessage(uint16_t keyValue) : Kinematics::NetworkMessage("input"), m_KeyValue(keyValue)
		{
		}

		virtual void Serialize(Kinematics::IPacket& p) { p& m_KeyValue; };
		virtual void Serialize(Kinematics::OPacket& p) { p& m_KeyValue; };

		uint16_t GetKey() { return m_KeyValue; }

		NETWORK_MESSAGE_TYPE(InputMessage);

	protected:
		uint16_t m_KeyValue;
	};

	class DataMessage : public Kinematics::NetworkMessage
	{
	public:
		DataMessage() : Kinematics::NetworkMessage("data")
		{
		}

		DataMessage(std::vector<Creature> objects) : Kinematics::NetworkMessage("data"), m_Objects(objects)
		{
		}

		virtual void Serialize(Kinematics::IPacket& p) 
		{
			size_t size = m_Objects.size();
			p& size;			
			m_Objects.resize(size);

			for (int i = 0; i < size; i++)
			{
				p& m_Objects[i];
			}
		};


		virtual void Serialize(Kinematics::OPacket& p) 
		{
			size_t size = m_Objects.size();
			p& size;

			for (auto obj : m_Objects)
			{
				p& obj;
			}
		};

		auto GetObjects() { return m_Objects; }

		NETWORK_MESSAGE_TYPE(DataMessage);

	protected:
		std::vector<Creature> m_Objects;
	};

	class CreatureUpdateMessage : public Kinematics::NetworkMessage
	{
	public:
		CreatureUpdateMessage() : Kinematics::NetworkMessage("creatureUpdate")
		{
		}		

		CreatureUpdateMessage(uint32_t id, int16_t x, int16_t y) : Kinematics::NetworkMessage("creatureUpdate"), m_ID(id), m_X(x), m_Y(y)
		{
		}

		virtual void Serialize(Kinematics::IPacket& p)
		{		
			p& m_ID;
			p& m_X;
			p& m_Y;
		};


		virtual void Serialize(Kinematics::OPacket& p)
		{			
			p& m_ID;
			p& m_X;
			p& m_Y;
		};

		uint32_t GetID() const { return m_ID; }
		int16_t GetX() const { return m_X; }
		int16_t GetY() const { return m_Y; }


		NETWORK_MESSAGE_TYPE(CreatureUpdateMessage);

	protected:
		uint32_t m_ID;
		int16_t m_X;
		int16_t m_Y;
	};

	CREATE_MESSAGE_FACTORY("input", InputMessage);
	CREATE_MESSAGE_FACTORY("data", DataMessage);
	CREATE_MESSAGE_FACTORY("creatureUpdate", CreatureUpdateMessage);
}
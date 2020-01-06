#pragma once

#include <Kinematics.h>
#include <vector>

#include "Game/Creatures/Creature.h"

namespace Game
{
	class GameConnectedMessage : public Kinematics::NetworkMessage
	{
	public:

		GameConnectedMessage() : Kinematics::NetworkMessage("gameConnected")
		{
		}

		GameConnectedMessage(uint32_t id) : Kinematics::NetworkMessage("gameConnected"), m_ID(id)
		{
		}

		uint32_t GetID() const { return m_ID; }

		virtual void Serialize(Kinematics::IPacket& p) { p& m_ID; };
		virtual void Serialize(Kinematics::OPacket& p) { p& m_ID; };

		NETWORK_MESSAGE_TYPE(GameConnectedMessage);

	private:
		uint32_t m_ID;
	};

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
				uint32_t id;
				Position pos;
				p& id;
				p& pos;

				m_Objects[i] = Creature(id, pos);
			}
		};


		virtual void Serialize(Kinematics::OPacket& p) 
		{
			size_t size = m_Objects.size();
			p& size;

			for (auto obj : m_Objects)
			{
				uint32_t id = obj.GetID();
				Position pos = obj.GetPosition();
				p& id;
				p& pos;
			}
		};

		auto GetObjects() { return m_Objects; }

		NETWORK_MESSAGE_TYPE(DataMessage);

	protected:
		std::vector<Creature> m_Objects;
	};

	class PlayerDisconnectMessage : public Kinematics::NetworkMessage
	{
	public:
		PlayerDisconnectMessage() : Kinematics::NetworkMessage("playerDisconnection")
		{
		}

		PlayerDisconnectMessage(uint32_t id) : Kinematics::NetworkMessage("playerDisconnection"), m_ID(id)
		{
		}

		uint32_t GetID() { return m_ID; }

		virtual void Serialize(Kinematics::IPacket& p) 
		{
			p& m_ID;
		}
		
		virtual void Serialize(Kinematics::OPacket& p) 
		{
			p& m_ID;
		}

	private:
		uint32_t m_ID;
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

	CREATE_MESSAGE_FACTORY("gameConnected", GameConnectedMessage);
	CREATE_MESSAGE_FACTORY("input", InputMessage);
	CREATE_MESSAGE_FACTORY("data", DataMessage);
	CREATE_MESSAGE_FACTORY("creatureUpdate", CreatureUpdateMessage);
	CREATE_MESSAGE_FACTORY("playerDisconnection", PlayerDisconnectMessage);
}
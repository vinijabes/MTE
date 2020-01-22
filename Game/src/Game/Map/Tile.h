#pragma once

#include "Position.h"

#include <list>

namespace Game {

	struct TileData
	{
		Position position;
	};

	class Tile
	{
	public:
		Tile() = default;
		Tile(const TileData & data)
			: m_Data(data)
		{}
		//TODO Return/Add items on tile
		//TODO Return/Add creatures on tile

		const Position& GetPosition() { return m_Data.position; }
		void SetPosition(const Position& position) { m_Data.position = position; }
		void SetPosition(const TileData& data) { m_Data.position = data.position; }

	public:
		static std::string RegisterScriptConstructor(Kinematics::Script& script)
		{
			return "Tile";
		}

		static void PushScriptInstance(Kinematics::Script& script, Tile* position)
		{
			script.SetMetaTable("Tile");
		}

		static Tile* GetTile(Kinematics::ScriptState& script, int32_t arg)
		{
			Kinematics::ScriptTable positionTable = script.Get().GetParameter<Kinematics::ScriptTable>(arg);
			Position p(positionTable.Get<int64_t>("x"), positionTable.Get<int64_t>("y"), positionTable.Get<int64_t>("z"));

			return nullptr;
		}

	protected:
		static int LuaTileCreate(Kinematics::ScriptState& script)
		{
			//PushScriptInstance();
			return 1;
		}

		static int LuaGetPosition(Kinematics::ScriptState& script)
		{
			//PushScriptInstance();
			return 1;
		}

	private:
		TileData m_Data;
	};
}
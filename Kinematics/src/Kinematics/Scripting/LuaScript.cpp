#include "mtepch.h"
#include "LuaScript.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Kinematics {
	LuaScript::LuaScript(const std::string& path)
		: Script(ScriptLang::LUA), L(nullptr), m_Push(0), m_Cp(false)
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		int status = luaL_loadfile(L, path.c_str());
		if (status)
		{
			KINEMATICS_CORE_ERROR("Error: script not loaded ({}, {})", path, status);
			L = 0;
		}
	}

	LuaScript::~LuaScript()
	{
		if (L && !m_Cp) lua_close(L);
	}

	void LuaScript::Clean()
	{
		int n = lua_gettop(L);
		lua_pop(L, n);
	}

	std::vector<std::string> LuaScript::LuaGetTableKeys(const std::string& name)
	{
		std::string code =
			"function getKeys(name) "
			"s = \"\""
			"for k, v in pairs(_G[name]) do "
			"    s = s..k..\",\" "
			"    end "
			"return s "
			"end";

		luaL_loadstring(L, code.c_str());
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, "getKeys");
		lua_pushstring(L, name.c_str());
		lua_pcall(L, 1, 1, 0);

		std::string test = lua_tostring(L, -1);
		std::vector<std::string> strings;
		std::string temp = "";;
		for (unsigned int i = 0; i < test.size(); i++)
		{
			if (test.at(i) != ',')
			{
				temp += test.at(i);
			}
			else
			{
				strings.push_back(temp);
				temp = "";
			}
		}

		Clean();
		return strings;
	}

	void LuaScript::Run()
	{
		int code = lua_pcall(L, 0, 0, 0);
		if (code) KINEMATICS_ERROR("LuaScript: Error {}", code);
	}

	bool LuaScript::Push(const std::string& varName, int& level)
	{
		level = 0;
		std::string var = "";

		for (unsigned int i = 0; i < varName.size(); i++)
		{
			if (varName.at(i) == '.')
			{
				if (level == 0)
				{
					lua_getglobal(L, var.c_str());
				}
				else
				{
					lua_getfield(L, -1, var.c_str());
				}

				if (lua_isnil(L, -1))
				{
					KINEMATICS_CORE_ERROR("LuaScript: {} is not defined", varName);
					return false;
				}
				else
				{
					var = "";
					level++;
				}
			}
			else
			{
				var += varName.at(i);
			}
		}

		if (level == 0)
		{
			lua_getglobal(L, var.c_str());
		}
		else
		{
			lua_getfield(L, -1, var.c_str());
		}

		if (lua_isnil(L, -1))
		{
			KINEMATICS_CORE_ERROR("LuaScript: {} is not defined", varName);
			return false;
		}

		return true;
	}

	void LuaScript::Pop(int level)
	{
		lua_pop(L, level);
	}

	bool LuaScript::GetBool(const std::string& var)
	{
		bool result = false;
		int level = 0;

		if (Push(var, level))
		{
			result = lua_toboolean(L, -1);
		}

		Pop(level + 1);
		return result;
	}

	int LuaScript::GetInt(const std::string& var)
	{
		int result = 0;
		int level = 0;

		if (Push(var, level))
		{
			if (lua_isnumber(L, -1))
				result = (int)lua_tonumber(L, -1);
			else
				KINEMATICS_CORE_ERROR("LuaScript: {} is not a number", var);
		}

		Pop(level + 1);
		return result;
	}

	float LuaScript::GetFloat(const std::string& var)
	{
		float result = 0;
		int level = 0;

		if (Push(var, level))
		{
			if (lua_isnumber(L, -1))
				result = (float)lua_tonumber(L, -1);
			else
				KINEMATICS_CORE_ERROR("LuaScript: {} is not a number", var);
		}

		Pop(level + 1);
		return result;
	}

	ScriptTable LuaScript::GetTable(const std::string& var)
	{
		int level = 0;

		if (Push(var, level))
		{
			return GetTable();
		}

		return ScriptTable();
	}

	ScriptTable LuaScript::GetTable()
	{
		ScriptTable table;

		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2) != 0)
			{
				KINEMATICS_TRACE("LuaTable: {} {}", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
				if (lua_type(L, -2) == LUA_TSTRING)
					switch (lua_type(L, -1))
					{
						case LUA_TNUMBER:
							table.Push(lua_tostring(L, -2), (float)lua_tonumber(L, -1));
							break;
						case LUA_TTABLE:
							table.Push(lua_tostring(L, -2), GetTable());
							break;
					}
				else
					switch (lua_type(L, -1))
					{
						case LUA_TNUMBER:
							table.Push(std::to_string((int)lua_tonumber(L, -2)), (float)lua_tonumber(L, -1));
							break;
						case LUA_TTABLE:
							table.Push(std::to_string((int)lua_tonumber(L, -2)), GetTable());
							break;
					}

				lua_pop(L, 1);
			}
		}

		return table;
	}

	std::string LuaScript::GetString(const std::string& var)
	{
		std::string result = "null";
		int level = 0;

		if (Push(var, level))
		{
			if (lua_isstring(L, -1))
				result = std::string(lua_tostring(L, -1));
			else
				KINEMATICS_CORE_ERROR("LuaScript: {} is not a string", var);
		}

		Pop(level + 1);
		return result;
	}
	std::vector<int> LuaScript::GetIntVector(const std::string& var)
	{
		std::vector<int> v;
		int level;

		if (Push(var, level))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				v.push_back((int)lua_tonumber(L, -1));
				lua_pop(L, 1);
			}

			Clean();
		}

		return v;
	}

	void LuaScript::PushInt(const int& var)
	{
		lua_pushnumber(L, var);
		++m_Push;
	}

	void LuaScript::PushFloat(const float& var)
	{
		lua_pushnumber(L, var);
		++m_Push;
	}

	void LuaScript::SetFunc(const std::string& var)
	{
		lua_getglobal(L, var.c_str());
	}

	int LuaScript::InternalCallInt()
	{
		lua_pcall(L, m_Push, LUA_MULTRET, 0);

		int result = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);

		m_Push = 0;
		return result;
	}

	void LuaScript::InternalCallVoid()
	{
		lua_pcall(L, m_Push, 0, 0);
		m_Push = 0;
	}

	ScriptTable LuaScript::InternalCallTable()
	{
		lua_pcall(L, m_Push, LUA_MULTRET, 0);

		ScriptTable result = GetTable();

		m_Push = 0;
		return result;
	}

	void LuaScript::RegisterCFunc(ScriptWrapperContainer& func, const std::string& name)
	{
		auto f = static_cast<LuaScriptWrapper*>(&func);
		lua_pushcfunction(L, f->m_Function);
		lua_setglobal(L, name.c_str());
	}

	void LuaScript::RegisterCFunc(ScriptWrapperContainer& func)
	{
		auto f = static_cast<LuaScriptWrapper*>(&func);
		lua_pushcfunction(L, f->m_Function);
		lua_setglobal(L, f->m_Name.c_str());
	}

	int LuaScript::GetIntParameter(int pos)
	{
		return (int)luaL_checkinteger(L, pos);
	}

	float LuaScript::GetFloatParameter(int pos)
	{
		return (float)luaL_checknumber(L, pos);
	}

	bool LuaScript::GetBoolParameter(int pos)
	{
		return false;
	}

	void* LuaScript::GetUserDataParameter(int pos, const std::string& meta)
	{
		return luaL_checkudata(L, pos, meta.c_str());
	}

	void LuaScript::CreateMetaTable(const std::string& name,
		ScriptWrapperContainer& newObj,
		ScriptWrapperContainer& deleteObj,
		const std::vector<Ref<ScriptWrapperContainer>>& methods)
	{
		lua_register(L, name.c_str(), static_cast<LuaScriptWrapper*>(&newObj)->m_Function);
		luaL_newmetatable(L, name.c_str());
		lua_pushcfunction(L, static_cast<LuaScriptWrapper*>(&deleteObj)->m_Function); lua_setfield(L, -2, "__gc");
		lua_pushvalue(L, -1); lua_setfield(L, -2, "__index");
		for (auto m : methods)
		{
			auto f = static_cast<LuaScriptWrapper*>(m.get());
			lua_pushcfunction(L, f->m_Function); 
			lua_setfield(L, -2, f->m_Name.c_str());
		}
		lua_pop(L, 1);
	}

	void LuaScript::SetMetaTable(const std::string& name)
	{
		luaL_setmetatable(L, name.c_str());
	}

	void* LuaScript::CreateUserData(size_t size)
	{
		return lua_newuserdata(L, size);
	}

	void* LuaScript::GetUserData(int index)
	{
		return lua_touserdata(L, index);
	}
}
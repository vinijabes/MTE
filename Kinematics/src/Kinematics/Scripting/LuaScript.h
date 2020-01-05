#pragma once

#include "Script.h"

struct lua_State;

namespace Kinematics {

	typedef int (*LuaCPPF)(ScriptState&);

	template< LuaCPPF Function >
	int LuaWrapper(lua_State* state)
	{
		auto script = LuaScript(state);
		ScriptState stateW(script);
		int result = Function(stateW);

		return result;
	}

	typedef int (*lua_CFunction) (lua_State* L);
	struct LuaScriptWrapper : public ScriptWrapperContainer
	{
		LuaScriptWrapper(lua_CFunction f) : m_Function(f), m_Name("") {}
		LuaScriptWrapper(lua_CFunction f, std::string name) : m_Function(f), m_Name(name) {}
		
		lua_CFunction m_Function;
		std::string m_Name;
	};

	class LuaScript : public Script
	{
	public:
		LuaScript(lua_State* state) : Script(ScriptLang::LUA), L(state), m_Cp(true) {}
		LuaScript(const std::string& path);
		~LuaScript();

		void Clean();

		std::vector<int> LuaGetVector(const std::string& name);
		std::vector<std::string> LuaGetTableKeys(const std::string& name);

		static Ref<LuaScript> Create(std::string path) { return CreateRef<LuaScript>(path); }

		virtual void Run() override;

		template<LuaCPPF Func>
		static LuaScriptWrapper* WrapPointer(const std::string& name)
		{
			return new LuaScriptWrapper(LuaWrapper<Func>, name);
		}

		template<LuaCPPF Func>
		static LuaScriptWrapper Wrap(const std::string& name)
		{
			return LuaScriptWrapper(LuaWrapper<Func>, name);
		}

		template<LuaCPPF Func>
		static LuaScriptWrapper Wrap()
		{
			return LuaScriptWrapper(LuaWrapper<Func>);
		}

	private:
		lua_State* L;
		bool m_Cp;

		int m_Push;

		virtual bool Push(const std::string& var, int& level) override;
		virtual void Pop(int level) override;

		virtual bool GetBool(const std::string& var) override;

		virtual int GetInt(const std::string& var) override;
		virtual float GetFloat(const std::string& var) override;

		virtual ScriptTable GetTable(const std::string& var) override;
		ScriptTable GetTable();

		virtual std::string GetString(const std::string& var) override;
		virtual std::vector<int> GetIntVector(const std::string& var) override;

		virtual void PushInt(const int& var) override;
		virtual void PushFloat(const float& var) override;

		virtual void SetFunc(const std::string& var);
		virtual int InternalCallInt() override;
		virtual void InternalCallVoid() override;
		virtual ScriptTable InternalCallTable() override;

		virtual void RegisterCFunc(ScriptWrapperContainer& func, const std::string& name) override;
		virtual void RegisterCFunc(ScriptWrapperContainer& func) override;

		virtual int GetIntParameter(int pos) override;
		virtual float GetFloatParameter(int pos) override;
		virtual bool GetBoolParameter(int pos) override;

		virtual void CreateMetaTable(const std::string& name,
			ScriptWrapperContainer& newObj,
			ScriptWrapperContainer& deleteObj,
			const std::vector<Ref<ScriptWrapperContainer>>& methods) override;
		virtual void SetMetaTable(const std::string& name) override;
		virtual void* GetUserDataParameter(int pos, const std::string& meta) override;

		virtual void* CreateUserData(size_t size) override;
		virtual void* GetUserData(int index) override;
	};
}


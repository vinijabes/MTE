#pragma once

#include "Script.h"

struct lua_State;

namespace Kinematics {

	typedef int (*LuaCPPF)(ScriptState&);
	class LuaScript;

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

		template <typename C>
		std::string RegisterObject()
		{
			return C::RegisterScriptConstructor(*this);
		}

		template <typename C>
		void PushUserObject(const C& var)
		{
			return C::PushScriptInstance(*this, var);
		}

		template<LuaCPPF Func>
		static LuaScriptWrapper* WrapPointer(const std::string& name)
		{
			return new LuaScriptWrapper(LuaWrapper<Func>, name);
		}

		template<LuaCPPF Func>
		static LuaScriptWrapper* WrapPointer()
		{
			return new LuaScriptWrapper(LuaWrapper<Func>);
		}

		template<LuaCPPF Func>
		static Ref<LuaScriptWrapper> WrapRef(const std::string& name)
		{
			return CreateRef<LuaScriptWrapper>(LuaWrapper<Func>, name);
		}

		template<LuaCPPF Func>
		static Ref<LuaScriptWrapper> WrapRef()
		{
			return CreateRef<LuaScriptWrapper>(LuaWrapper<Func>);
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
		virtual int64_t GetField(int arg, const std::string& key) override;


		virtual ScriptTable GetTable(const std::string& var) override;
		ScriptTable GetTable();

		virtual std::string GetString(const std::string& var) override;
		virtual std::vector<int> GetIntVector(const std::string& var) override;

		virtual void PushInt(const int& var) override;
		void PushInt64(const int64_t& var);
		virtual void PushFloat(const float& var) override;
		virtual void PushBoolean(const bool& var) override;
		virtual void PushTable(const ScriptTable& var) override;

		virtual void SetFunc(const std::string& var);
		virtual int InternalCallInt(const int& push) override;
		virtual void InternalCallVoid(const int& push) override;
		virtual ScriptTable InternalCallTable(const int& push) override;

		virtual void RegisterCFunc(ScriptWrapperContainer& func, const std::string& name) override;
		virtual void RegisterCFunc(ScriptWrapperContainer& func) override;

		virtual int GetIntParameter(int pos) override;
		virtual float GetFloatParameter(int pos) override;
		ScriptTable GetTableParameter(int pos) override;

		virtual void CreateMetaTable(const std::string& name,
			Ref<ScriptWrapperContainer> newObj,
			Ref<ScriptWrapperContainer> deleteObj,
			const std::vector<Ref<ScriptWrapperContainer>>& methods) override;
		virtual void SetMetaTable(const std::string& name) override;
		virtual void* GetUserDataParameter(int pos, const std::string& meta) override;

		virtual void* CreateUserData(size_t size) override;
		virtual void* GetUserData(int index) override;
	};
}


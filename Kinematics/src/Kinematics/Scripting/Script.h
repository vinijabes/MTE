#pragma once

#include "Kinematics/Core/Core.h"
#include <string>

namespace Kinematics {

	class LuaScript;
	class ScriptState;

	enum class ScriptVarType
	{
		STRING,
		FLOAT,
		INT,
		NUMBER,
		BOOL,
		TABLE
	};

	enum class ScriptLang
	{
		LUA
	};

	class ScriptValueInterface
	{
	public:
		ScriptValueInterface(ScriptVarType type) : m_Type(type) {}
		ScriptVarType GetType() const { return m_Type; }

	private:
		ScriptVarType m_Type;
	};

	template <typename T>
	class ScriptValue : public ScriptValueInterface
	{
		friend class ScriptTable;
	public:
		ScriptValue(ScriptVarType type, const T& value)
			: ScriptValueInterface(type), m_Value(value)
		{
		}

		T Get()
		{
			return m_Value;
		}

		T Get() const
		{
			return m_Value;
		}

	private:
		T m_Value;
	};

	class ScriptTable
	{
	public:

		void Push(std::string key, const std::string& value)
		{
			m_KeyPairs[key] = std::static_pointer_cast<ScriptValueInterface>(CreateRef<ScriptValue<std::string>>(ScriptValue(ScriptVarType::STRING, value)));
		}

		void Push(std::string key, const float& value)
		{
			m_KeyPairs[key] = std::static_pointer_cast<ScriptValueInterface>(CreateRef<ScriptValue<float>>(ScriptValue(ScriptVarType::FLOAT, value)));
		}

		void Push(std::string key, const int64_t& value)
		{
			m_KeyPairs[key] = std::static_pointer_cast<ScriptValueInterface>(CreateRef<ScriptValue<int64_t>>(ScriptValue(ScriptVarType::NUMBER, value)));
		}

		void Push(std::string key, const ScriptTable& value)
		{
			m_KeyPairs[key] = std::static_pointer_cast<ScriptValueInterface>(CreateRef<ScriptValue<ScriptTable>>(ScriptValue(ScriptVarType::TABLE, value)));
		}

		ScriptTable operator[](const std::string& key)
		{
			return (std::static_pointer_cast<ScriptValue<ScriptTable>>(m_KeyPairs[key]))->Get();
		}

		template<typename T>
		T Get(const std::string& key)
		{
			return (std::static_pointer_cast<ScriptValue<T>>(m_KeyPairs[key]))->Get();
		}

		auto begin() const { return m_KeyPairs.begin(); }
		auto end() const { return m_KeyPairs.end(); }

	private:
		std::unordered_map<std::string, Ref<ScriptValueInterface>> m_KeyPairs;
	};

	struct ScriptWrapperContainer
	{
	};

	typedef std::vector<ScriptWrapperContainer> ScriptLib;

	class Script
	{
	public:
		Script(ScriptLang lang) : m_Lang(lang) {}
		static Ref<Script> Create(std::string path);

		virtual bool Push(const std::string& var, int& level) = 0;
		virtual void Pop(int level) = 0;

		virtual void SetFunc(const std::string& var) = 0;

		template <typename T>
		T Get(const std::string& var);
		
		virtual int64_t GetField(int arg, const std::string& key) = 0;

		template <typename T>
		T GetParameter(int pos);

		template <typename T>
		T** GetUserDataParameter(int pos, const std::string& meta)
		{
			return reinterpret_cast<T**>(GetUserDataParameter(pos, meta));
		}

		virtual void* GetUserDataParameter(int pos, const std::string& meta) = 0;

		template <typename T>
		void Push(const T& var);
		
		template <typename T>
		void Push(T* var);

		template <typename T>
		T Call(const std::string& func)
		{
			SetFunc(func);
			return Call<T>();
		}

		template <typename T, typename ... Types>
		T Call(const std::string& func, Types ... args)
		{
			SetFunc(func);
			PushCallArgument(args...);
			return Call<T>(sizeof...(Types));
		}

		template <typename T>
		T Call(int push = 0);

		virtual void Run() = 0;
		virtual void RegisterCFunc(ScriptWrapperContainer& func, const std::string& name) = 0;
		virtual void RegisterCFunc(ScriptWrapperContainer& func) = 0;

		void RegisterLib(ScriptLib lib)
		{
			for (auto m : lib)
				RegisterCFunc(m);
		}

		/*METADATA HANDLING*/
		virtual void CreateMetaTable(const std::string& name,
			Ref<ScriptWrapperContainer> newObj,
			Ref<ScriptWrapperContainer> deleteObj,
			const std::vector<Ref<ScriptWrapperContainer>>& methods) = 0;
		virtual void SetMetaTable(const std::string& name) = 0;

		template <typename T>
		T** CreateUserData()
		{
			return reinterpret_cast<T**>(CreateUserData(sizeof(T)));
		}
		virtual void* CreateUserData(size_t size) = 0;

		template <typename T>
		T** GetUserData(int index)
		{
			return reinterpret_cast<T**>(GetUserData(index));
		}
		virtual void* GetUserData(int index) = 0;

		template <typename C>
		constexpr void RegisterObject();

	protected:
		template <typename T, typename ... Types>
		void PushCallArgument(T value)
		{
			Push<T>(value);
		}

		template <typename T, typename ... Types>
		void PushCallArgument(T value, Types ... args)
		{
			PushCallArgument(value);
			PushCallArgument(args...);
		}

		template<typename T>
		bool IsTypeRegistered()
		{
			return m_RegisteredTypes.find(typeid(T).hash_code()) != m_RegisteredTypes.end();
		}

	protected:
		/*VAR HANDLING*/
		virtual bool GetBool(const std::string& var) = 0;

		virtual int GetInt(const std::string& var) = 0;
		virtual float GetFloat(const std::string& var) = 0;

		virtual ScriptTable GetTable(const std::string& var) = 0;

		virtual std::string GetString(const std::string& var) = 0;

		virtual std::vector<int> GetIntVector(const std::string& var) = 0;

		/*FUNCTION HANDLING*/
		virtual void PushInt(const int& var) = 0;
		virtual void PushFloat(const float& var) = 0;
		virtual void PushBoolean(const bool& var) = 0;
		virtual void PushTable(const ScriptTable& var) = 0;

		virtual int InternalCallInt(const int& push) = 0;
		virtual void InternalCallVoid(const int& push) = 0;
		virtual ScriptTable InternalCallTable(const int& push) = 0;

		virtual int GetIntParameter(int pos) = 0;
		virtual float GetFloatParameter(int pos) = 0;
		virtual ScriptTable GetTableParameter(int pos) = 0;
	protected:
		std::unordered_map<size_t, std::string> m_RegisteredTypes;
	private:
		ScriptLang m_Lang;
	};

	class ScriptState
	{
	public:
		ScriptState(Script& script) : m_Script(script) {}
		Script& Get() { return m_Script; }

	private:
		Script& m_Script;
	};

	template<>
	inline bool Script::Get(const std::string& var)
	{
		return GetBool(var);
	}

	template<>
	inline int Script::Get(const std::string& var)
	{
		return GetInt(var);
	}

	template<>
	inline float Script::Get(const std::string& var)
	{
		return GetFloat(var);
	}

	template<>
	inline std::string Script::Get(const std::string& var)
	{
		return GetString(var);
	}

	template<>
	inline std::vector<int> Script::Get(const std::string& var)
	{
		return GetIntVector(var);
	}

	template<>
	inline ScriptTable Script::Get(const std::string& var)
	{
		return GetTable(var);
	}

	template<>
	inline int Script::GetParameter(int pos)
	{
		return GetIntParameter(pos);
	}

	template<>
	inline float Script::GetParameter(int pos)
	{
		return GetFloatParameter(pos);
	}

	template<>
	inline ScriptTable Script::GetParameter(int pos)
	{
		return GetTableParameter(pos);
	}

	template<>
	inline void Script::Push(const int& var)
	{
		PushInt(var);
	}

	template<>
	inline void Script::Push(const float& var)
	{
		PushFloat(var);
	}
	
	template<>
	inline void Script::Push(const bool& var)
	{
		PushBoolean(var);
	}

	template<>
	inline void Script::Push(const ScriptTable& var)
	{
		PushTable(var);
	}


	template<typename T>
	inline void Script::Push(const T& var)
	{
		if (!IsTypeRegistered<T>())
		{
			RegisterObject<T>();
		}

		if (m_Lang == ScriptLang::LUA)
			static_cast<LuaScript*>(this)->template PushUserObject<T>(var);
	}

	template<typename T>
	inline void Script::Push(T* var)
	{
		if (!IsTypeRegistered<T>())
		{
			RegisterObject<T>();
		}

		if (m_Lang == ScriptLang::LUA)
			static_cast<LuaScript*>(this)->template PushUserObject<T>(var);
	}

	template <>
	inline int Script::Call(int push)
	{
		return InternalCallInt(push);
	}

	template<typename C>
	inline constexpr void Script::RegisterObject()
	{
		if (IsTypeRegistered<C>())
		{
			KINEMATICS_CORE_TRACE("Script::RegisterObject: {} already registered", typeid(C).name());
			return;
		}

		if (m_Lang == ScriptLang::LUA)
			m_RegisteredTypes[typeid(C).hash_code()] = static_cast<LuaScript*>(this)->template RegisterObject<C>();
	}

	template <>
	inline ScriptTable Script::Call(int push)
	{
		return InternalCallTable(push);
	}

	template <>
	inline void Script::Call(int push)
	{
		return InternalCallVoid(push);
	}

}
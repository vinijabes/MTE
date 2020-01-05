#pragma once

#include "Kinematics/Core/Core.h"
#include <string>

namespace Kinematics {

	enum class ScriptVarType
	{
		STRING,
		FLOAT,
		INT,
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
			m_KeyPairs[key] = std::static_pointer_cast<ScriptValueInterface>(CreateRef<ScriptValue<float>>(ScriptValue(ScriptVarType::TABLE, value)));
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

	private:
		std::unordered_map<std::string, Ref<ScriptValueInterface>> m_KeyPairs;
	};


	class ScriptState;
	struct ScriptWrapperContainer
	{
	};

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

		template <typename T>
		T GetParameter(int pos);

		template <typename T>
		T* GetUserDataParameter(int pos, const std::string& meta)
		{
			return (T*) GetUserDataParameter(pos, meta);
		}

		virtual void* GetUserDataParameter(int pos, const std::string& meta) = 0;

		template <typename T>
		void Push(const T& var);

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
			return Call<T>();
		}

		template <typename T>
		T Call();

		virtual void Run() = 0;
		virtual void RegisterCFunc(ScriptWrapperContainer& func, const std::string& name) = 0;
		virtual void RegisterCFunc(ScriptWrapperContainer& func) = 0;

		/*METADATA HANDLING*/
		virtual void CreateMetaTable(const std::string& name,
			ScriptWrapperContainer& newObj,
			ScriptWrapperContainer& deleteObj,
			const std::vector<Ref<ScriptWrapperContainer>>& methods) = 0;
		virtual void SetMetaTable(const std::string& name) = 0;

		template <typename T>
		void* CreateUserData()
		{
			return CreateUserData(sizeof(T));
		}
		virtual void* CreateUserData(size_t size) = 0;

		template <typename T>
		T* GetUserData(int index)
		{
			return (T*) GetUserData(index);
		}
		virtual void* GetUserData(int index) = 0;

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

		virtual int InternalCallInt() = 0;
		virtual void InternalCallVoid() = 0;
		virtual ScriptTable InternalCallTable() = 0;

		virtual int GetIntParameter(int pos) = 0;
		virtual float GetFloatParameter(int pos) = 0;
		virtual bool GetBoolParameter(int pos) = 0;

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
	inline void Script::Push(const int& var)
	{
		PushInt(var);
	}

	template<>
	inline void Script::Push(const float& var)
	{
		PushFloat(var);
	}

	template <>
	inline int Script::Call()
	{
		return InternalCallInt();
	}

	template <>
	inline ScriptTable Script::Call()
	{
		return InternalCallTable();
	}

	template <>
	inline void Script::Call()
	{
		return InternalCallVoid();
	}

}
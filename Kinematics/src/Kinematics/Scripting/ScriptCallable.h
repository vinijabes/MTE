#pragma once

#include "Script.h"
#include <list>

namespace Kinematics {

	
	class ScriptCallableInterface
	{
	public:
		ScriptCallableInterface(Ref<Script> script, std::string func) : m_Script(script), m_FuncName(func)
		{ }

		template<typename T, typename ... Types>
		T operator()(Types ... args)
		{
			return m_Script->Call<T>(args...);
		}
	protected:
		Ref<Script> m_Script;
		std::string m_FuncName;
	};
	
	template<typename T, typename ... Types>
	class ScriptCallable : public ScriptCallableInterface
	{
	public:
		ScriptCallable(Ref<Script> script, std::string func) : ScriptCallableInterface(script, func)
		{ }

		T operator()(Types ... args)
		{
			return m_Script->Call<T>(m_FuncName, args...);
		}
	};
}
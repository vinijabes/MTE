#pragma once

#include <functional>
#include "ScriptCallable.h"


namespace Kinematics {

	class Callable
	{ 
	};

	template<typename R, typename ... Types>
	class CallbackInterface : public Callable
	{
	public:
		virtual R operator()(Types ... args) = 0;
	};

	template<typename R, typename ... Types>
	class LuaCallback : public CallbackInterface<R, Types...>
	{
	public:
		LuaCallback(ScriptCallable<R, Types...> callable) : m_Callable(callable)
		{

		}

		virtual R operator()(Types ...args) override
		{
			return m_Callable(args...);
		}

	private:
		ScriptCallable<R, Types...> m_Callable;
	};

	template<typename R, typename ... Types>
	class FunctionCallback : public CallbackInterface<R, Types...>
	{
	public:
		FunctionCallback()
		{

		}

		FunctionCallback(std::function<R(Types...)> callable) : m_Callable(callable)
		{

		}

		template <typename Func>
		FunctionCallback(Func callable) : m_Callable(std::function<R(Types...)> (callable))
		{

		}

		virtual R operator()(Types ...args) override
		{
			return m_Callable(args...);
		}

	private:
		std::function<R(Types...)> m_Callable;
	};

	template<typename R, typename ... Types>
	class Callback
	{
	public:
		void operator()(Types ... args)
		{
			for (auto f : m_Functions)
				(*f)(args...);
		}

		Callback operator+(const Ref<CallbackInterface<R, Types...>>& other)
		{
			this->m_Functions.push_back(other);
			return *this;
		}

	private:
		std::list<Ref<CallbackInterface<R, Types...>>> m_Functions;
	};

}
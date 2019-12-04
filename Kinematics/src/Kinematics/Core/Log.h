#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ava {
	class AVA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define AVA_CORE_TRACE(...)		::Ava::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AVA_CORE_INFO(...)		::Ava::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AVA_CORE_WARN(...)		::Ava::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AVA_CORE_ERROR(...)		::Ava::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AVA_CORE_FATAL(...)		::Ava::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define AVA_TRACE(...)		::Ava::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AVA_INFO(...)		::Ava::Log::GetClientLogger()->info(__VA_ARGS__)
#define AVA_WARN(...)		::Ava::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AVA_ERROR(...)		::Ava::Log::GetClientLogger()->error(__VA_ARGS__)
#define AVA_FATAL(...)		::Ava::Log::GetClientLogger()->fatal(__VA_ARGS__)
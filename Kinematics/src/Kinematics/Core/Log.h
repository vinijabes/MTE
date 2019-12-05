#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Kinematics {
	class KINEMATICS_API Log
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
#define KINEMATICS_CORE_TRACE(...)		::Kinematics::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KINEMATICS_CORE_INFO(...)		::Kinematics::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KINEMATICS_CORE_WARN(...)		::Kinematics::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KINEMATICS_CORE_ERROR(...)		::Kinematics::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KINEMATICS_CORE_FATAL(...)		::Kinematics::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define KINEMATICS_TRACE(...)		::Kinematics::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KINEMATICS_INFO(...)		::Kinematics::Log::GetClientLogger()->info(__VA_ARGS__)
#define KINEMATICS_WARN(...)		::Kinematics::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KINEMATICS_ERROR(...)		::Kinematics::Log::GetClientLogger()->error(__VA_ARGS__)
#define KINEMATICS_FATAL(...)		::Kinematics::Log::GetClientLogger()->fatal(__VA_ARGS__)
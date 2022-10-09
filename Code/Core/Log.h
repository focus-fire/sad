#pragma once

#include <spdlog/spdlog.h>

enum class ELogType
{
	Assert,
	Info,
	Debug,
	Warn,
};

namespace core
{
#ifdef _SAD_ENABLE_LOGGING
	void InitializeLogging();
	void KillLogging();

	void Log(const ELogType type, const char* message);

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args)
	{
		if constexpr (sizeof...(args) > 0)
		{
			// Use 'fmt' formatting provided by spdlog to parse variadic arguments
			// ex: Log(ELogType::Debug, "The meaning of life is {}", 42);
			// https://github.com/fmtlib/fmt 
			fmt::format_args fmtArgs = fmt::make_format_args(args...);
			std::string fmtString = fmt::vformat(message, fmtArgs);
			core::Log(type, fmtString.c_str());
		}
		else
		{
			core::Log(type, message);
		}
	}
#else
	inline void InitializeLogging() { }
	inline void KillLogging() { }

	inline void Log(const ELogType type, const char* message) { }

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args) { }
#endif
}

#pragma once

#include <spdlog/spdlog.h>

#include "Base.h"

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

	// Use a macro-based assert to trigger file/line specific debugbreaks
	#define SAD_ASSERT(check, message) { if(!(check)) { core::Log(ELogType::Assert, "{} @ {} line {}", message, SAD_FILE_NAME, __LINE__); SAD_DEBUGBREAK(); } }
	#define SAD_LAZY_ASSERT(check) { SAD_EXPAND_MACRO( SAD_ASSERT(check, SAD_STRINGIFY_MACRO(check)) ) }
#else
	void InitializeLogging() { }
	void KillLogging() { }

	void Log(const ELogType type, const char* message) { }

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args) { }

	#define SAD_ASSERT(check, message) { }
	#define SAD_LAZY_ASSERT(check) { }
#endif
}

#pragma once

#include <spdlog/spdlog.h>

/**
 * @brief Specifies the type of log and/or assert being created
*/
enum class ELogType
{
	Assert,
	Error,
	Warn,
	Info,
	Debug,
	Trace,
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

	/**
	 * @brief Appends a sink pointer to the logging system
	 * @param sink Valid pointer to a created spdlog sink
	*/
	void AddLoggingSink(spdlog::sink_ptr sink);
#else
	inline void InitializeLogging() { }
	inline void KillLogging() { }

	inline void Log(const ELogType type, const char* message) { }

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args) { }

	void AddLoggingSink(spdlog::sink_ptr sink) { }
#endif
}

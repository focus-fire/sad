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
	/**
	 * @brief Creates loggers and populates sinks when the engine launches
	 * @note Sets up multi-threaded console logger, single-threaded file logger, and msvc logger on windows
	*/
	void InitializeLogging();

	/**
	 * @brief Destroys loggers if they're initialized and shutsdown spdlog 
	*/
	void KillLogging();

	/**
	 * @brief Base logging class used to send a single message to a logging channel
	 * @param type Type of log to submit to the logger
	 * @param message Message to print in the log
	*/
	void Log(const ELogType type, const char* message);

	/**
	 * @brief Variadic logging implementation that prints a message containing the paramters passed 
	 * @tparam ...TArgs Variadic arguments passed to print in the fmt formatted message string
	 * @param type Type of log to submit to the logger
	 * @param message fmt formatted message to print in the log
	 * @param ...args Arguments to pass to the fmt-formatted string
	 * @sample core::Log(ELogType::Debug, "This is a {}, not a {}, or a {}", foo, bar, baz);
	*/
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

	/**
	 * @brief Removes a sink pointer from the logging system 
	 * @param sink Valid eointer to a spdlog sink
	*/
	void RemoveLoggingSink(spdlog::sink_ptr sink);
#else
	inline void InitializeLogging() { }
	inline void KillLogging() { }

	inline void Log(const ELogType type, const char* message) { }

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args) { }

	inline void AddLoggingSink(spdlog::sink_ptr sink) { }
	inline void RemoveLoggingSink(spdlog::sink_ptr sink) { }
#endif
}

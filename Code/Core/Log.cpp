#include "sadpch.h"
#include "Log.h"

#ifdef _SAD_ENABLE_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _SAD_WINDOWS
#include <spdlog/sinks/msvc_sink.h>
#endif

namespace
{
	static bool s_IsInitialized = false;
	std::shared_ptr<spdlog::logger> s_DebugLogger;
	std::shared_ptr<spdlog::logger> s_AssertLogger;

	// Change these logging levels for different levels of information
	// ie: Changing the debug level to 'trace' will view trace logs
	const spdlog::level::level_enum c_DebugLoggerLevel = spdlog::level::debug;
	const spdlog::level::level_enum c_AssertLoggerLevel = spdlog::level::err;
}

void core::InitializeLogging()
{
	if (!s_IsInitialized)
	{
		s_IsInitialized = true;

		// Logs follow the format: [MM-DD-YY HH:MM:SS.mm] [level] message
		std::string debugLogPattern = "[%m-%d-%C %X.%e] %^[%l] %v%$";

		// Asserts follow the format: [MM-DD-YY HH:MM:SS.mm] [logger] message
		std::string assertLogPattern = "[%m-%d-%C %X.%e] %^[%n] %v%$";

		// Standard logger outputs all logs (except trace) to console and file
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> debugConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::shared_ptr<spdlog::sinks::basic_file_sink_st> debugFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/debug.txt", true);
		debugConsoleSink->set_level(c_DebugLoggerLevel);
		debugFileSink->set_level(c_DebugLoggerLevel);

		// Assert logger should only log critical asserts to both the console and file
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> assertConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::shared_ptr<spdlog::sinks::basic_file_sink_st> assertFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/asserts.txt", true);
		assertConsoleSink->set_level(c_AssertLoggerLevel);
		assertFileSink->set_level(c_AssertLoggerLevel);

		// Create sink vectors for both log types
		std::vector<spdlog::sink_ptr> debugSinks { debugConsoleSink, debugFileSink };
		std::vector<spdlog::sink_ptr> assertSinks { assertConsoleSink, assertFileSink };

#ifdef _SAD_WINDOWS
		// Create a sink for the VisualStudio 'Output' window
		std::shared_ptr<spdlog::sinks::msvc_sink_mt> vsOutputSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		vsOutputSink->set_level(c_DebugLoggerLevel);

		debugSinks.push_back(vsOutputSink);
		assertSinks.push_back(vsOutputSink);
#endif

		// Set sinks, log levels, and pattern defaults for both loggers
		s_DebugLogger = std::make_shared<spdlog::logger>("debug", debugSinks.begin(), debugSinks.end());
		s_DebugLogger->set_level(c_DebugLoggerLevel);
		s_DebugLogger->set_pattern(debugLogPattern);

		s_AssertLogger = std::make_shared<spdlog::logger>("assert", assertSinks.begin(), assertSinks.end());
		s_AssertLogger->set_level(c_AssertLoggerLevel);
		s_AssertLogger->set_pattern(assertLogPattern);
	}
}

void core::KillLogging()
{
	if (s_IsInitialized)
	{
		s_IsInitialized = false;

		s_AssertLogger->flush();
		s_DebugLogger->flush();
	}
}

void core::Log(const ELogType type, const char* message)
{
	switch (type)
	{
	case ELogType::Assert:
		s_AssertLogger->error(message);
		break;
	case ELogType::Error:
		s_DebugLogger->critical(message);
		break;
	case ELogType::Warn:
		s_DebugLogger->warn(message);
		break;
	case ELogType::Info:
		s_DebugLogger->info(message);
		break;
	case ELogType::Debug:
		s_DebugLogger->debug(message);
		break;
	case ELogType::Trace:
		s_DebugLogger->trace(message);
		break;
	}
}

#endif

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

	// Logs follow the format: [MM-DD-YY HH:MM:SS.mm] [level] message
	std::string c_DebugLogPattern = "[%m-%d-%C %X.%e] %^[%l] %v%$";

	// Asserts follow the format: [MM-DD-YY HH:MM:SS.mm] [logger] message
	std::string c_AssertLogPattern = "[%m-%d-%C %X.%e] %^[%n] %v%$";
}

void core::InitializeLogging()
{
	if (!s_IsInitialized)
	{
		s_IsInitialized = true;

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
		s_DebugLogger->set_pattern(c_DebugLogPattern);

		s_AssertLogger = std::make_shared<spdlog::logger>("assert", assertSinks.begin(), assertSinks.end());
		s_AssertLogger->set_level(c_AssertLoggerLevel);
		s_AssertLogger->set_pattern(c_AssertLogPattern);

		spdlog::set_default_logger(s_DebugLogger);
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

	spdlog::shutdown();
}

void core::Log(const ELogType type, const char* message)
{
	// Prevent log from executing if no logging sinks have been created
	if (!s_DebugLogger || !s_AssertLogger)
		return;

	SAD_ASSERT(message && message[0], "Congratulations! You managed to log an empty string.");

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

void core::AddLoggingSink(spdlog::sink_ptr sink)
{
	// Any sinks added after the logs are initialized will adopt the debug log pattern
	sink->set_pattern(c_DebugLogPattern);

	s_DebugLogger->sinks().push_back(sink);
	core::Log(ELogType::Trace, "[Log] Successfully added debug logging sink");

	s_AssertLogger->sinks().push_back(sink);
	core::Log(ELogType::Trace, "[Log] Successfully added assert logging sink");
}

void core::RemoveLoggingSink(spdlog::sink_ptr sink)
{
	std::vector<spdlog::sink_ptr>& debugSinks = s_DebugLogger->sinks();
	std::vector<spdlog::sink_ptr>& assertSinks = s_AssertLogger->sinks();

	auto debugIt = std::find(debugSinks.begin(), debugSinks.end(), sink);
	auto assertIt = std::find(assertSinks.begin(), assertSinks.end(), sink);

	if (debugIt != debugSinks.end())
	{
		debugSinks.erase(debugIt);
		core::Log(ELogType::Trace, "[Log] Successfully removed debug logging sink");
	}

	if (assertIt != assertSinks.end())
	{
		assertSinks.erase(assertIt);
		core::Log(ELogType::Trace, "[Log] Successfully removed assert logging sink");
	}
}

#endif

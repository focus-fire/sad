#include "Log.h"

#ifdef _SAD_ENABLE_LOGGING
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _SAD_WINDOWS
#include <spdlog/sinks/msvc_sink.h>
#endif

#include "Base.h"

namespace
{
	static bool s_IsInitialized = false;
	std::shared_ptr<spdlog::logger> s_DebugLogger;
	std::shared_ptr<spdlog::logger> s_AssertLogger;
}

void core::InitializeLogging()
{
	if (!s_IsInitialized)
	{
		s_IsInitialized = true;

		// Logs follow the format -> [MM-DD-YY HH:MM:SS.mm] [logger] message
		std::string logPattern = "[%m-%d-%C %X.%e] %^[%n] %v%$";

#ifdef _SAD_WINDOWS
		// Create a sink for the VisualStudio 'Output' window
		std::shared_ptr<spdlog::sinks::msvc_sink_mt> vsOutputSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		vsOutputSink->set_level(spdlog::level::debug);
#endif

		// Standard logger outputs all logs to console and file
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> coreConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::shared_ptr<spdlog::sinks::basic_file_sink_st> coreFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/debug.txt", true);
		coreConsoleSink->set_level(spdlog::level::debug);
		coreFileSink->set_level(spdlog::level::debug);

		spdlog::sinks_init_list coreSinkList = { coreFileSink, coreConsoleSink, vsOutputSink };
		s_DebugLogger = std::make_shared<spdlog::logger>("debug", begin(coreSinkList), end(coreSinkList));
		s_DebugLogger->set_level(spdlog::level::debug);
		s_DebugLogger->set_pattern(logPattern);

		// Assert logger should only log critical asserts to both the console and file
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> assertConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::shared_ptr<spdlog::sinks::basic_file_sink_st> assertFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/asserts.txt", true);
		assertConsoleSink->set_level(spdlog::level::err);
		assertFileSink->set_level(spdlog::level::err);

		spdlog::sinks_init_list assertSinkList = { assertConsoleSink, assertFileSink, vsOutputSink };
		s_AssertLogger = std::make_shared<spdlog::logger>("assert", begin(assertSinkList), end(assertSinkList));
		s_AssertLogger->set_level(spdlog::level::err);
		s_AssertLogger->set_pattern(logPattern);
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
	case ELogType::Info:
		s_DebugLogger->info(message);
		break;
	case ELogType::Debug:
		s_DebugLogger->debug(message);
		break;
	case ELogType::Warn:
		s_DebugLogger->warn(message);
		break;
	}
}

#endif

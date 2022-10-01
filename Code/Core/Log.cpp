#include "Log.h"

#ifdef _SAD_ENABLE_LOGGING
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _SAD_WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

namespace
{
	static bool s_IsInitialized = false;
	std::shared_ptr<spdlog::logger> s_CoreLogger;
	std::shared_ptr<spdlog::logger> s_AssertLogger;
}

void core::InitializeLogging()
{
	if (!s_IsInitialized)
	{
		s_IsInitialized = true;

		// Logs follow the format -> [MM-DD-YY HH:MM:SS.mm] [logger] message
		std::string logPattern = "[%m-%d-%C %X.%e] %^[%n] %v%$";

		// Standard logger outputs to console and file
		auto coreConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto coreFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/debug.txt", true);
		coreConsoleSink->set_level(spdlog::level::info);
		coreFileSink->set_level(spdlog::level::info);

#ifdef _SAD_WIN32
		auto vsOutputSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		vsOutputSink->set_level(spdlog::level::info);
#endif

		spdlog::sinks_init_list coreSinkList = { coreFileSink, coreConsoleSink, vsOutputSink };
		s_CoreLogger = std::make_shared<spdlog::logger>("log", begin(coreSinkList), end(coreSinkList));
		s_CoreLogger->set_level(spdlog::level::debug);
		s_CoreLogger->set_pattern(logPattern);

		// Assert logger should only log critical asserts
		auto assertConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto assertFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/asserts.txt", true);
		assertConsoleSink->set_level(spdlog::level::critical);
		assertFileSink->set_level(spdlog::level::critical);

		spdlog::sinks_init_list assertSinkList = { assertConsoleSink, assertFileSink, vsOutputSink };
		s_AssertLogger = std::make_shared<spdlog::logger>("assert", begin(assertSinkList), end(assertSinkList));
		s_AssertLogger->set_level(spdlog::level::critical);
		s_AssertLogger->set_pattern(logPattern);
	}
}

void core::KillLogging()
{
	if (s_IsInitialized)
	{
		s_IsInitialized = false;

		s_AssertLogger->flush();
		s_CoreLogger->flush();
	}
}

void core::Log(const ELogType type, const char* message)
{
	switch (type)
	{
	case ELogType::Assert:
		s_AssertLogger->critical(message);
		break;
	case ELogType::Info:
		s_CoreLogger->info(message);
		break;
	case ELogType::Debug:
		s_CoreLogger->debug(message);
		break;
	case ELogType::Warn:
		s_CoreLogger->warn(message);
		break;
	}
}

#endif

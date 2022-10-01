#pragma once

#include <iostream>
#include <memory>

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
		const std::size_t numberOfArgs = sizeof...(args);

		// TODO: Handle variadic log function
		if (numberOfArgs > 0)
		{
		}
		else
		{
			core::Log(type, message);
		}
	}
#else
	void InitializeLogging() { }
	void KillLogging() { }

	void Log(const ELogType type, const char* message) { }
#endif
}

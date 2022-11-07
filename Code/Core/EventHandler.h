#pragma once

#include <functional>

namespace core
{
	struct EventListener
	{
		std::vector<std::function<void()>> eventFunction;
		bool inUpdateLoop;
	};

	void InitializeListener(const char* name, std::function<void()> eventFunction, bool inUpdateLoop = false);
	void RemoveListener(const char* name);
	void SignalEvent(const char* name);

	void UpdateEvents();

}
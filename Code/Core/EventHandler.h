#pragma once

namespace core
{
	struct EventListener
	{
		std::vector<void (*)()> eventFunction;
		bool inUpdateLoop;
	};

	void InitializeListener(const char* name, void (* eventFunction)(), bool inUpdateLoop = false);
	void RemoveListener(const char* name);
	void SignalEvent(const char* name);

	void UpdateEvents();

}
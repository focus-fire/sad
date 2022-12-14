#include "sadpch.h"

std::unordered_map<const char*, core::EventListener> Listeners;

void core::InitializeListener(const char* name, std::function<void(void)> eventFunction, bool inUpdateLoop)
{
	// If a listener with the same name already exists, push the callback to it's eventFunction list
	if (Listeners.count(name)>0)
	{
		if (Listeners[name].inUpdateLoop != inUpdateLoop)
		{
			core::Log(ELogType::Warn, "Event listener initialization attempted with a different update state than it's parent listener, ignoring initialization");
			return;
		}
		Listeners[name].eventFunction.push_back({ eventFunction });
	}
	// Otherwise, create a new listener
	else
	{
		EventListener eventListener;
		eventListener = { { eventFunction }, inUpdateLoop };
		Listeners[name] = eventListener;
	}
}

void core::SignalEvent(const char* name)
{
	// Loop through callbacks within the listener
	for (auto& eventFunction : Listeners[name].eventFunction)
	{
		eventFunction();
	}
}

void core::RemoveListener(const char* name)
{
	Listeners.erase(name);
}

void core::UpdateEvents()
{
	// Loop through all listeners with inUpdateLoop true
	for (auto &eventListener : Listeners)
	{
		if (eventListener.second.inUpdateLoop)
		{
			// Loop through callbacks within the listener
			for (auto &eventFunction : eventListener.second.eventFunction)
			{
				eventFunction();
			}
		}
	}
}
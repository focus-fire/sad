/*

Used:
https://linuxhint.com/callback-function-in-cpp/
https://www.youtube.com/watch?v=IiKyRcLnrP0&ab_channel=DylanFalconer

Other Resources:
https://riptutorial.com/cplusplus/example/24695/observer-pattern
https://gameprogrammingpatterns.com/observer.html
https://isetta.io/blogs/week-9/#event-messaging-system

*/

#include "sadpch.h"

struct EventListener {
	std::vector<void (*)()> eventFunction;
	bool inUpdateLoop;
};

std::unordered_map<const char*, EventListener> Listeners;

void core::InitializeListener(const char* name, void(*eventFunction)(), bool inUpdateLoop)
{
	// If a listener with the same name already exists, push the callback to it's eventFunction list
	if (Listeners.count(name)>0)
	{
		Listeners[name].eventFunction.push_back({ *eventFunction });
	}
	// Otherwise, create a new listener
	else
	{
		EventListener eventListener;
		eventListener = { { *eventFunction }, inUpdateLoop };
		Listeners[name] = eventListener;
	}
}

void core::RemoveListener(const char* name)
{
	Listeners.erase(name);
}

void core::SignalEvent(const char* name)
{
	// Loop through callbacks within the listener
	for (auto &eventFunction : Listeners[name].eventFunction)
	{
		eventFunction();
	}
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
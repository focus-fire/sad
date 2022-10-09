/*

Jake comments:
if we're using multi-threading on the engine side events are essential
ie: We launch the editor and run the data importer on level.json, after each Entity is hit the ResourceManager then loads in the thrown resource on a separate thread
but to communicate that we'd have to throw an event

Used:
https://linuxhint.com/callback-function-in-cpp/
https://www.youtube.com/watch?v=IiKyRcLnrP0&ab_channel=DylanFalconer

Other Resources:
https://riptutorial.com/cplusplus/example/24695/observer-pattern
https://gameprogrammingpatterns.com/observer.html
https://isetta.io/blogs/week-9/#event-messaging-system

*/

#include "sadpch.h"

struct eventListener {
	const char* m_Name;
	void (* m_Event)();
	int m_Amount;
	bool m_Persist;
};

std::list<eventListener> Listeners;

const char* currentEvent;

void core::InitializeListener(const char* m_Name, void(*m_Event)(), int m_Amount, bool m_Persist)
{
	eventListener eventFunction;
	eventFunction = { m_Name, *m_Event, m_Amount, m_Persist };

	Listeners.push_back(eventFunction);
}

bool FindListener(eventListener value)
{
	return value.m_Name == currentEvent;
}

void core::RemoveListener(const char* m_Name)
{
	currentEvent = m_Name;
	Listeners.remove_if(FindListener);
}

void core::SignalEvent(const char* m_Name)
{
	currentEvent = m_Name;
	for (eventListener& eventFunction : Listeners)
	{
		eventFunction.m_Event();
	}
}

void core::UpdateEvents()
{
	for (eventListener& eventFunction : Listeners)
	{
		if (eventFunction.m_Persist)
		{
			eventFunction.m_Event();
			eventFunction.m_Amount--;

			if (eventFunction.m_Amount == 0)
			{
				core::RemoveListener(eventFunction.m_Name);
				break;
			}
		}
	}
}
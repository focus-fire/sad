#pragma once

#include <functional>

namespace core
{
	struct EventListener
	{
		std::vector<std::function<void(void)>> eventFunction;
		bool inUpdateLoop;
	};

	/**
	 * @brief Initializes an event listener for a function
	 * @param name const char* Key name used to signal
	 * @param eventFunction void Function
	 * @param inUpdateLoop bool Optional parameter which is default false. Setting to true adds this listener to the event update loop which updates every frame 
	*/
	void InitializeListener(const char* name, std::function<void(void)> eventFunction, bool inUpdateLoop = false);

	/**
	 * @brief Delete a whole event listener
	 * @param name const char* Key Name
	*/
	void RemoveListener(const char* name);

	/**
	 * @brief Signal all event functions within a listener
	 * @param name const char* Key Name
	*/
	void SignalEvent(const char* name);

	void UpdateEvents();
}
#include <catch2/catch.hpp>

#include <Core/EventHandler.h>

#pragma optimize("", off)

TEST_CASE("Events can be initialized successfully")
{
	SECTION("single listener")
	{
		bool eventTriggered = false;

		core::InitializeListener("TestSingleListener", [&]() { eventTriggered = true; });
		core::SignalEvent("TestSingleListener");

		REQUIRE(eventTriggered);
	}

	SECTION("multiple listeners")
	{
		bool firstEventTriggered = false;
		bool secondEventTriggered = false;

		core::InitializeListener("TestMultipleListeners", [&]() { firstEventTriggered = true; });
		core::InitializeListener("TestMultipleListeners", [&]() { secondEventTriggered = true; });
		core::SignalEvent("TestMultipleListeners");

		REQUIRE(firstEventTriggered);
		REQUIRE(secondEventTriggered);
	}

	SECTION("multiple listeners, different events")
	{
		bool firstListenerTriggered = false;
		bool secondListenerTriggered = false;

		core::InitializeListener("TestFirstListener", [&]() { firstListenerTriggered = true; });
		core::SignalEvent("TestFirstListener");

		REQUIRE(firstListenerTriggered);

		core::InitializeListener("TestSecondListener", [&]() { secondListenerTriggered = true; });
		core::SignalEvent("TestSecondListener");

		REQUIRE(secondListenerTriggered);
	}
}
#include <catch2/catch.hpp>

/*
 * Disable optimize in unit tests for more verbose traces/errors
 */
#pragma optimize("", off)

/**
 * Catch2 v.2.x
 * https://github.com/catchorg/catch2/tree/v2.x
 * See https://github.com/catchorg/Catch2/tree/v2.x/examples for more examples
 */

namespace core
{
	unsigned int Division(unsigned int number1, unsigned int number2)
	{
		return number1 / number2;
	}
}

TEST_CASE("Dividing numbers", "[division]")
{
	REQUIRE(core::Division(2, 2) == 1);
	REQUIRE(core::Division(12, 3) == 4);
	REQUIRE(core::Division(33, 11) == 3);
	REQUIRE(core::Division(100, 5) == 20);
}

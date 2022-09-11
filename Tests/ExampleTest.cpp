#include <catch2/catch.hpp>

/**
 * Catch2 v.2.x
 * https://github.com/catchorg/catch2/tree/v2.x
 * See https://github.com/catchorg/Catch2/tree/v2.x/examples for more examples
 */

namespace sad
{
	unsigned int Factorial(unsigned int number)
	{
		return number <= 1 ? number : Factorial(number - 1) * number;
	}
}


TEST_CASE("Factorials are computed", "[factorial]")
{
	REQUIRE(sad::Factorial(1) == 1);
	REQUIRE(sad::Factorial(2) == 2);
	REQUIRE(sad::Factorial(3) == 6);
	REQUIRE(sad::Factorial(10) == 3628800);
}
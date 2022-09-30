#include <catch2/catch.hpp>

/**
 * Catch2 v.2.x
 * https://github.com/catchorg/catch2/tree/v2.x
 * See https://github.com/catchorg/Catch2/tree/v2.x/examples for more examples
 */

namespace sad 
{
	unsigned int Multiplication(unsigned int number1, unsigned int number2) 
	{
		return number1 * number2;
	}
}

TEST_CASE("Multiplying numbers", "[multiplication]") 
{
	REQUIRE(sad::Multiplication(2, 2) == 4);
	REQUIRE(sad::Multiplication(3, 3) == 9);
	REQUIRE(sad::Multiplication(4, 4) == 16);
	REQUIRE(sad::Multiplication(5, 5) == 25);
}

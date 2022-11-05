#include <catch2/catch.hpp>

#include <Core/StringUtils.h>

#pragma optimize("", off)

TEST_CASE("Equals properly evaluates string equality")
{
	std::string a = "Equals properly evaluates string equality";

	SECTION("strings are equal")
	{
		std::string b = "Equals properly evaluates string equality";
		REQUIRE(core::StringUtils::Equals(a, b));
	}

	SECTION("strings are not equal")
	{
		std::string b = "Equals does not evaluate string equality";
		REQUIRE_FALSE(core::StringUtils::Equals(a, b));
	}
}

TEST_CASE("Strings are trimmed properly")
{
	std::string sample = "  The quick brown fox jumps over the lazy dog  ";

	SECTION("left/right trim is performed correctly")
	{
		std::string result = core::StringUtils::Trim(sample);

		REQUIRE(result == "The quick brown fox jumps over the lazy dog");
	}

	SECTION("right trim is performed correctly")
	{
		std::string result = core::StringUtils::RightTrim(sample);

		REQUIRE(result == "  The quick brown fox jumps over the lazy dog");
	}

	SECTION("left trim is performed correctly")
	{
		std::string result = core::StringUtils::LeftTrim(sample);

		REQUIRE(result == "The quick brown fox jumps over the lazy dog  ");
	}
}

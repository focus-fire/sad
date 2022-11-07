#include <catch2/catch.hpp>

#include <vector>

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

TEST_CASE("Strings can be split by a delimiter properly")
{
	std::string expectedValues[4] = { "Bet...", "Bet", "Bet?", "Bet!" };

	SECTION("comma delimiter (csv)")
	{
		std::string csv = "Bet...,Bet,Bet?,Bet!";
		
		std::vector<std::string> values = core::StringUtils::Split(',', csv);
		for (int i = 0; i < 4; ++i)
			REQUIRE(core::StringUtils::Equals(values[i], expectedValues[i]));
	}

	SECTION("pipe delimiter (psv)")
	{
		std::string psv = "Bet...|Bet|Bet?|Bet!";
		
		std::vector<std::string> values = core::StringUtils::Split('|', psv);
		for (int i = 0; i < 4; ++i)
			REQUIRE(core::StringUtils::Equals(values[i], expectedValues[i]));
	}

	SECTION("tab delimiter (tsv)")
	{
		std::string tsv = "Bet...\tBet\tBet?\tBet!";
		
		std::vector<std::string> values = core::StringUtils::Split('\t', tsv);
		for (int i = 0; i < 4; ++i)
			REQUIRE(core::StringUtils::Equals(values[i], expectedValues[i]));
	}

	SECTION("space delimiter (ssv?)")
	{
		std::string ssv = "Bet... Bet Bet? Bet!";
		
		std::vector<std::string> values = core::StringUtils::Split(' ', ssv);
		for (int i = 0; i < 4; ++i)
			REQUIRE(core::StringUtils::Equals(values[i], expectedValues[i]));
	}

	SECTION("semi-colon delimiter (scsv?)")
	{
		std::string scsv = "Bet...;Bet;Bet?;Bet!";
		
		std::vector<std::string> values = core::StringUtils::Split(';', scsv);
		for (int i = 0; i < 4; ++i)
			REQUIRE(core::StringUtils::Equals(values[i], expectedValues[i]));
	}
}

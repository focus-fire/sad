#include <catch2/catch.hpp>

#include <Core/MathUtils.h>

#pragma optimize("", off)

TEST_CASE("RoundTo properly rounds numbers")
{
	double value = 694.943564;

	SECTION("thousandths")
	{
		double thousandths = core::MathUtils::RoundTo(value, 0.001);
		REQUIRE(core::MathUtils::CompareFloatingPoints(thousandths, 694.944));
	}

	SECTION("hundredths")
	{
		double hundredths = core::MathUtils::RoundTo(value, 0.01);
		REQUIRE(core::MathUtils::CompareFloatingPoints(hundredths, 694.94));
	}
	
	SECTION("tenths")
	{
		double tenths = core::MathUtils::RoundTo(value, 0.1);
		REQUIRE(core::MathUtils::CompareFloatingPoints(tenths, 694.9));
	}

	SECTION("ones")
	{
		double ones = core::MathUtils::RoundTo(value, 1);
		REQUIRE(core::MathUtils::CompareFloatingPoints(ones, 695));
	}

	SECTION("tens")
	{
		double tens = core::MathUtils::RoundTo(value, 10);
		REQUIRE(core::MathUtils::CompareFloatingPoints(tens, 690));
	}

	SECTION("hundreds")
	{
		double hundreds = core::MathUtils::RoundTo(value, 100);
		REQUIRE(core::MathUtils::CompareFloatingPoints(hundreds, 700));
	}

	SECTION("thousands")
	{
		double thousands = core::MathUtils::RoundTo(value, 1000);
		REQUIRE(core::MathUtils::CompareFloatingPoints(thousands, 1000));
	}
}

TEST_CASE("ComparingFloatingPoints compares floating point numbers properly")
{
	SECTION("doubles")
	{
		double x = 678.987;
		double y = 678.987;

		REQUIRE(core::MathUtils::CompareFloatingPoints(x, y, 0.001));
	}

	SECTION("floats")
	{
		float x = 678.9f;
		float y = 678.9f;

		REQUIRE(core::MathUtils::CompareFloatingPoints(x, y, 0.1));
	}
}

TEST_CASE("DegToRad properly converts degrees to radians")
{
	SECTION("zero")
	{
		float deg = 0.0f;
		float rad = core::MathUtils::DegToRad(deg);

		REQUIRE(core::MathUtils::CompareFloatingPoints(rad, 0.0f));
	}

	SECTION("basic trig wheel values")
	{
		float deg = 90.0f;
		float rad = core::MathUtils::DegToRad(deg);

		REQUIRE(core::MathUtils::CompareFloatingPoints(rad, 1.5708, 0.0001));

		deg = 180.0f;
		rad = core::MathUtils::DegToRad(deg);

		REQUIRE(core::MathUtils::CompareFloatingPoints(rad, c_PI, 0.0001));

		deg = 270.0f;
		rad = core::MathUtils::DegToRad(deg);

		REQUIRE(core::MathUtils::CompareFloatingPoints(rad, 4.7123, 0.0001));

		deg = 360.0f;
		rad = core::MathUtils::DegToRad(deg);

		REQUIRE(core::MathUtils::CompareFloatingPoints(rad, c_2PI, 0.0001));
	}
}

TEST_CASE("Clamp properly clamps numbers")
{
	SECTION("raising a value")
	{
		float x = 8.0f;
		float min = 10.0f;
		float max = 12.0f;

		float result = core::MathUtils::Clamp(x, min, max);

		REQUIRE(result == min);
	}

	SECTION("flooring a value")
	{
		float x = 20.0f;
		float min = 10;
		float max = 12;

		float result = core::MathUtils::Clamp(x, min, max);

		REQUIRE(result == max);
	}

	SECTION("using an exact value")
	{
		float x = 11.0f;
		float min = 10;
		float max = 12;

		float result = core::MathUtils::Clamp(x, min, max);
		 
		REQUIRE(result == x);
	}
}
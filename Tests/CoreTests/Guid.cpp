#include <catch2/catch.hpp>

#include <Core/Guid.h>
#include <Core/StringUtils.h>

#pragma optimize("", off)

TEST_CASE("Guids can be constructed properly")
{
	const core::Guid guid = core::Guid::CreateGuid();

	REQUIRE_FALSE(guid.IsNull());
}

TEST_CASE("Guids have a valid ToString()")
{
	const core::Guid guid = core::Guid::CreateGuid();
	const std::string stringGuid = guid.ToString();

	REQUIRE_FALSE(guid.IsNull());
	REQUIRE_FALSE(stringGuid.empty());
}

TEST_CASE("Guids can be properly compared")
{
	SECTION("constant guids can be compared")
	{
		const core::Guid firstGuid = core::Guid::CreateGuid();
		const core::Guid secondGuid = core::Guid::CreateGuid();
		REQUIRE_FALSE(firstGuid == secondGuid);

		const core::Guid firstGuidCopy = firstGuid;
		REQUIRE(firstGuid == firstGuidCopy);
	}

	SECTION("mutable guids can be compared")
	{
		core::Guid firstGuid = core::Guid::CreateGuid();
		core::Guid secondGuid = core::Guid::CreateGuid();

		REQUIRE_FALSE(firstGuid == secondGuid);
		REQUIRE_FALSE(firstGuid.Equals(secondGuid));
		REQUIRE_FALSE(core::StringUtils::Equals(firstGuid.ToString(), secondGuid.ToString()));

		core::Guid firstGuidCopy = firstGuid;

		REQUIRE(firstGuid == firstGuidCopy);
		REQUIRE(firstGuid.Equals(firstGuidCopy));
		REQUIRE(core::StringUtils::Equals(firstGuid.ToString(), firstGuidCopy.ToString()));
	}
}

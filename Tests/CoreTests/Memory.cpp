#include <catch2/catch.hpp>

#include <string>

#include <Engine/Transform.h>
#include <Core/Memory.h>

#pragma optimize("", off)

TEST_CASE("Pointer reference type creates a non-null shared pointer")
{
	SECTION("std::string")
	{
		std::string testString = "test";
		core::Pointer<std::string> smartString = core::CreatePointer<std::string>(testString);

		REQUIRE(smartString);

		REQUIRE(smartString.get()->compare(testString) == 0);
		REQUIRE_FALSE(smartString.get()->empty());
	}

	SECTION("sad::Transform")
	{
		sad::Transform transform = sad::Transform();
		core::Pointer<sad::Transform> transformPointer = core::CreatePointer<sad::Transform>(transform);

		REQUIRE(transformPointer);

		REQUIRE(transformPointer.get()->GetPosition() == transform.GetPosition());
		REQUIRE(transformPointer.get()->GetRotation() == transform.GetRotation());
		REQUIRE(transformPointer.get()->GetScale() == transform.GetScale());
		REQUIRE(transformPointer.get()->GetTransformMatrix() == transform.GetTransformMatrix());
	}
}

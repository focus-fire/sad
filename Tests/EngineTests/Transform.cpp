#include <catch2/catch.hpp>

#include <Engine/Transform.h>

#pragma optimize("", off)

TEST_CASE("Transform getters and setters should retrieve or modify the correct data")
{
	sad::Transform transform = sad::Transform();

	SECTION("default position data")
	{
		REQUIRE(transform.GetPosition() == glm::vec3(0.0f));
	}

	SECTION("default rotation data")
	{
		REQUIRE(transform.GetRotation() == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	}

	SECTION("default scale data")
	{
		REQUIRE(transform.GetScale() == glm::vec3(1.0f));
	}

	SECTION("altered position data")
	{
		glm::vec3 pos = glm::vec3(1.0f);
		transform.SetPosition(pos);

		REQUIRE_FALSE(transform.GetPosition() == glm::vec3(0.0f));
		REQUIRE(transform.GetPosition() == pos);
	}

	SECTION("altered rotation data with quaternion")
	{
		glm::quat rot = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);
		transform.SetRotation(rot);

		REQUIRE_FALSE(transform.GetRotation() == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(transform.GetRotation() == rot);
	}
}

TEST_CASE("Transforms should be initialized properly")
{
	SECTION("default initialization")
	{
		sad::Transform transform = sad::Transform();

		REQUIRE(transform.GetPosition() == glm::vec3(0.0f));
		REQUIRE(transform.GetRotation() == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(transform.GetScale() == glm::vec3(1.0f));
	}

	SECTION("default initialization with parameters (quat)")
	{
		glm::vec3 pos = glm::vec3(1.0f);
		glm::quat rot = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 scl = glm::vec3(2.0f);

		sad::Transform transform = sad::Transform(pos, rot, scl);

		glm::vec3 newPos = transform.GetPosition();
		glm::quat newRot = transform.GetRotation();
		glm::vec3 newScl = transform.GetScale();

		REQUIRE_FALSE(newPos == glm::vec3(0.0f));
		REQUIRE(newPos == pos);

		REQUIRE_FALSE(newRot == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(newRot == rot);

		REQUIRE_FALSE(newScl == glm::vec3(1.0f));
		REQUIRE(newScl == scl);
	}

	SECTION("default initialization with parameters (vec3)")
	{
		glm::vec3 pos = glm::vec3(1.0f);
		glm::quat rot = glm::vec3(90.0f, 90.0f, 90.0f);
		glm::vec3 scl = glm::vec3(2.0f);

		sad::Transform transform = sad::Transform(pos, rot, scl);

		glm::vec3 newPos = transform.GetPosition();
		glm::quat newRot = transform.GetRotation();
		glm::vec3 newScl = transform.GetScale();

		REQUIRE_FALSE(newPos == glm::vec3(0.0f));
		REQUIRE(newPos == pos);

		REQUIRE_FALSE(newRot == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(newRot == rot);

		REQUIRE_FALSE(newScl == glm::vec3(1.0f));
		REQUIRE(newScl == scl);
	}
}

TEST_CASE("Transform translations should apply properly")
{
	sad::Transform transform = sad::Transform();

	SECTION("standard vector offset")
	{
		glm::vec3 offset = glm::vec3(1.0f, 2.0f, 3.0f);
		
		transform.Translate(offset);

		glm::vec3 newPos = transform.GetPosition();
		REQUIRE_FALSE(newPos == glm::vec3(0.0f));
		REQUIRE(newPos == offset);
	}
	
	SECTION("multiple vector offsets")
	{
		glm::vec3 offset1 = glm::vec3(1.0f, 2.0f, 3.0f);
		glm::vec3 offset2 = glm::vec3(4.0f, 5.0f, 6.0f);

		transform.Translate(offset1);
		transform.Translate(offset2);

		glm::vec3 newPos = transform.GetPosition();
		REQUIRE_FALSE(newPos == glm::vec3(0.0f));
		REQUIRE(newPos == (offset1 + offset2));
	}
}

TEST_CASE("Transform rotations should apply properly")
{
	sad::Transform transform = sad::Transform();

	SECTION("standard vector rotation")
	{
		glm::vec3 rot = glm::vec3(90.0f, 90.0f, 90.0f);

		transform.Rotate(rot);

		glm::quat newRot = transform.GetRotation();
		REQUIRE_FALSE(newRot == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(newRot == glm::quat(glm::radians(rot)));
	}

	SECTION("standard quaternion rotation")
	{
		glm::quat rot = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);

		transform.Rotate(rot);

		glm::quat newRot = transform.GetRotation();
		REQUIRE_FALSE(newRot == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(newRot == rot);
	}

	SECTION("standard axis/angle rotation")
	{
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
		float angle = 90.0f;

		transform.Rotate(axis, angle);
		
		glm::quat newRot = transform.GetRotation();
		REQUIRE_FALSE(newRot == glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		REQUIRE(newRot == glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::radians(angle), axis));
	}
}

TEST_CASE("Transform scaling should apply properly")
{
	sad::Transform transform = sad::Transform();

	SECTION("standard vector scale")
	{
		glm::vec3 scl = glm::vec3(2.0f);

		transform.Scale(scl);

		glm::vec3 newScl = transform.GetScale();
		REQUIRE_FALSE(newScl == glm::vec3(1.0f));
		REQUIRE(newScl == scl);
	}

	SECTION("multiple vector scales")
	{
		glm::vec3 scl1 = glm::vec3(2.0f);
		glm::vec3 scl2 = glm::vec3(0.5f);

		transform.Scale(scl1);
		transform.Scale(scl2);

		glm::vec3 newScl = transform.GetScale();
		REQUIRE_FALSE(newScl == scl1);
		REQUIRE_FALSE(newScl == scl2);
		REQUIRE(newScl == glm::vec3(1.0f));
	}
}

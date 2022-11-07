#include <catch2/catch.hpp>

#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Registry.h>

#pragma optimize("", off)

TEST_CASE("Entity handles are contstructed and behave properly")
{
	sad::ecs::Entity entity = sad::ecs::Entity();
	
	SECTION("entity has a valid handle")
	{
		const entt::entity handle = entity.GetHandle();

		bool isValidHandle = handle != entt::null;
		REQUIRE(isValidHandle);
	}

	SECTION("entity handles can be compared successfully")
	{
		sad::ecs::Entity secondEntity = sad::ecs::Entity();

		REQUIRE(secondEntity == secondEntity);
		REQUIRE(entity != secondEntity);
	}
}

TEST_CASE("Entities can add components")
{
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	sad::ecs::Entity entity = sad::ecs::Entity();

	SECTION("entity can add a component")
	{
		struct TestComponent { int Data = 0; };
		entity.AddComponent<TestComponent>({ 5 });

		const TestComponent& testComponent = entity.GetComponent<TestComponent>();
		REQUIRE(testComponent.Data == 5);
	}

	SECTION("entity can add an empty/tag component")
	{
		struct TagComponent { };
		entity.AddEmptyComponent<TagComponent>({ });

		bool addedTag = false;
		auto view = world.view<const TagComponent>();
		for (auto [entity] : view.each())
			addedTag = true;

		REQUIRE(addedTag);
	}
}

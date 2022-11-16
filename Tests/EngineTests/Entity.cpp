#include <catch2/catch.hpp>

#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Registry.h>

#pragma optimize("", off)

TEST_CASE("Entity handles are contstructed and wrap properly")
{
	SECTION("entity has a valid handle")
	{
		const entt::entity handle = sad::ecs::Registry::GenerateEntityHandle();
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		bool isValidHandle = entity.GetHandle() != entt::null;
		REQUIRE(entity.GetHandle() != sad::ecs::Entity::Null().GetHandle());
	}

	SECTION("entity has a null handle")
	{
		sad::ecs::Entity entity = sad::ecs::Entity::Null();
		REQUIRE(!entity);
		REQUIRE_FALSE(entity);
	}

	SECTION("entity handles can be compared successfully")
	{
		const entt::entity firstHandle = sad::ecs::Registry::GenerateEntityHandle();
		sad::ecs::Entity firstEntity = sad::ecs::Entity(firstHandle);
		REQUIRE(firstEntity.GetHandle() == firstHandle);

		const entt::entity secondHandle = sad::ecs::Registry::GenerateEntityHandle();
		sad::ecs::Entity secondEntity = sad::ecs::Entity(secondHandle);
		REQUIRE(secondEntity.GetHandle() == secondHandle);
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

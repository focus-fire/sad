#include "sadpch.h"

#include "Level.h"

#include "Scripting/ScriptingEngine.h"

#include "ECS/Entity.h"
#include "ECS/Components/ComponentTypes.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ECS/Systems/BoundSystem.h"
#include "ECS/Systems/RenderableObjectSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"

void sad::Level::Start()
{
	cs::ScriptingEngine::RuntimeStart(this);

	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();
	auto view = world.view<ecs::ScriptComponent>();
	for (auto handle : view)
	{
		ecs::Entity entity = ecs::Entity(handle);

		// Scripting engine should handle instantiation of script methods
		// Also should handle validating that the provdied script still exists on the entity
		// tldr: this is what calls Awake() on all SadBehaviours
		cs::ScriptingEngine::AwakeSadBehaviourInstance(entity);
	}
}

void sad::Level::Update(sad::ecs::EntityWorld& world)
{
	// Update SadBehaviours
	auto view = world.view<ecs::ScriptComponent>();
	for (auto handle : view)
	{
		ecs::Entity entity = ecs::Entity(handle);

		// tldr: this is what calls Update() on all SadBehaviours
		cs::ScriptingEngine::UpdateSadBehaviourInstance(entity);
	}

	// Update non-gameplay ECS systems
	ecs::PlayerControllerSystem::Update(world);
	ecs::BoundSystem::Update(world);

	// Drawing 
	ecs::RenderableObjectSystem::Update(world);
	ecs::RenderingSystem::Draw(world);
}

void sad::Level::PopulateLevelGuids()
{
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// Loop through all valid entities and add them to the level cache
	world.each<>([&](const auto entity) 
	{
		ImportEntityFromHandle(entity);
	});
}

sad::ecs::Entity sad::Level::LookupEntityByGuid(const core::Guid& guid)
{
	if (m_EntityMap.find(guid) == m_EntityMap.end())
		return ecs::Entity::Null();

	return m_EntityMap[guid];
}

sad::ecs::Entity sad::Level::LookupEntityByName(const std::string& name)
{
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	bool isFound = false;
	ecs::Entity target;

	auto view = world.view<ecs::NameComponent>();
	for (auto [entity, nameComponent] : view.each())
	{
		// Search all name components for the first entity with a matching name
		if (core::StringUtils::Equals(name, nameComponent.m_Name))
		{
			isFound = true;
			target = entity;
			break;
		}
	}

	if (!isFound)
		return ecs::Entity::Null();

	return target;
}

sad::ecs::Entity sad::Level::InstantiateEntity(const std::string& name)
{
	entt::entity handle = ecs::Registry::GenerateEntityHandle();

	return InstantiateEntityFromHandle(handle, name);
}

sad::ecs::Entity sad::Level::InstantiateEntityFromHandle(entt::entity handle, const std::string& name, core::Guid guid /* = core::Guid::CreateGuid() */)
{
	ecs::Entity entity = ecs::Entity(handle);
	core::Pointer<sad::Transform> transform = core::CreatePointer<sad::Transform>();
	core::Pointer<sad::Bound> bound = core::CreatePointer<sad::Bound>(*transform.get());

	// All entities should have a GUID, Transform, and Bound
	entity.AddComponent<ecs::GuidComponent>({ guid });
	entity.AddComponent<ecs::NameComponent>({ name });
	entity.AddComponent<ecs::TransformComponent>({ transform });
	entity.AddComponent<ecs::BoundComponent>({ bound });

	m_EntityMap[guid] = entity;

	return entity;
}

sad::ecs::Entity sad::Level::ImportEntityFromHandle(entt::entity handle, core::Guid guid /* = core::Guid::CreateGuid() */)
{
	ecs::Entity entity = ecs::Entity(handle);

	if (!entity.HasComponent<ecs::GuidComponent>())
	{
		entity.OverwriteComponent<ecs::GuidComponent>({ guid });

		m_EntityMap[guid] = entity;
	}

	return entity;
}

bool sad::Level::DestroyEntity(sad::ecs::Entity entity)
{
	if (m_EntityMap.find(entity.GetGuid()) == m_EntityMap.end())
		return false;

	m_EntityMap.erase(entity.GetGuid());
	sad::ecs::Registry::EraseEntityHandle(entity);

	return true;
}

bool sad::Level::DestroyEntityByName(const std::string& name)
{
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	ecs::Entity target = LookupEntityByName(name);
	if (!target)
		return false;

	return DestroyEntity(target);
}

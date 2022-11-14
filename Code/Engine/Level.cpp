#include "sadpch.h"

#include "Level.h"

#include "Scripting/ScriptingEngine.h"

#include "ECS/Entity.h"
#include "ECS/Components/ComponentTypes.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ECS/Systems/BoundSystem.h"
#include "ECS/Systems/RenderableObjectSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"

void sad::Level::Update(sad::ecs::EntityWorld& world)
{
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

	world.each<>([&](const auto entity) 
	{
		ImportEntityFromHandle(entity);
	});
}

sad::ecs::Entity sad::Level::InstantiateEntity()
{
	entt::entity handle = ecs::Registry::GenerateEntityHandle();

	return InstantiateEntityFromHandle(handle);
}

sad::ecs::Entity sad::Level::InstantiateEntityFromHandle(entt::entity handle, core::Guid guid /* = core::Guid::CreateGuid() */)
{
	ecs::Entity entity = ecs::Entity(handle);
	core::Pointer<sad::Transform> transform = core::CreatePointer<sad::Transform>();
	core::Pointer<sad::Bound> bound = core::CreatePointer<sad::Bound>(*transform.get());

	// All entities should have a GUID, Transform, and Bound
	entity.AddComponent<ecs::GuidComponent>({ guid });
	entity.AddComponent<ecs::TransformComponent>({ transform });
	entity.AddComponent<ecs::BoundComponent>({ bound });

	m_EntityMap[guid] = entity;

	return entity;
}

sad::ecs::Entity sad::Level::ImportEntityFromHandle(entt::entity handle, core::Guid guid /* = core::Guid::CreateGuid() */)
{
	ecs::Entity entity = ecs::Entity(handle);
	entity.AddComponent<ecs::GuidComponent>({ guid });

	m_EntityMap[guid] = entity;

	return entity;
}

void sad::Level::DestroyEntity(sad::ecs::Entity entity)
{
	//sad::ecs::Registry::EraseEntityHandle(entity);
	m_EntityMap.erase(entity.GetGuid());
}


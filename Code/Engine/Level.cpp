#include "sadpch.h"

#include "Level.h"

#include "Application.h"

#include "Scripting/ScriptingEngine.h"

#include "ECS/Entity.h"
#include "ECS/Components/ComponentTypes.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ECS/Systems/BoundSystem.h"
#include "ECS/Systems/LightSystem.h"
#include "ECS/Systems/RenderableObjectSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"

void sad::Level::Start()
{
	cs::ScriptingEngine::RuntimeStart(this);
}

void sad::Level::Update(sad::ecs::EntityWorld& world)
{
	auto view = world.view<ecs::ScriptComponent>();
	for (auto [handle, scriptComponent] : view.each())
	{
		// Check if new scripts have been added in the editor and need to be instantiated
		// Also check if 'DrawGizmos' needs to be called on a script
		ecs::Entity entity = ecs::Entity(handle);
		if (!cs::ScriptingEngine::SadBehaviourInstanceExists(entity.GetGuid()))
			cs::ScriptingEngine::CreateNativeSadBehaviourInstance(entity);

		// Since script instance exists, call DrawGizmos()
		// This should be updated each frame and should only be enabled while in Editor mode
		if (sad::Application::s_EngineState->GetEngineMode() == EEngineMode::Editor)
			cs::ScriptingEngine::DrawGizmosForSadBehaviourInstance(entity);
	}

	// Update non-gameplay ECS systems
	ecs::PlayerControllerSystem::Update(world);
	ecs::BoundSystem::Update(world);

	// Drawing 
	// ecs::LightSystem::Update(world);
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

sad::ecs::Entity sad::Level::GetEntityByGuid(const core::NativeGuid& guid)
{
	return GetEntityByGuid(core::Guid(guid));
}

sad::ecs::Entity sad::Level::GetEntityByGuid(const core::Guid& guid)
{
	if (m_EntityMap.find(guid) == m_EntityMap.end())
		return ecs::Entity::Null();

	return m_EntityMap[guid];
}

sad::ecs::Entity sad::Level::GetEntityByName(const std::string& name)
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
	entity.AddComponent<ecs::GuidComponent>(guid);
	entity.AddComponent<ecs::NameComponent>(name);
	entity.AddComponent<ecs::TransformComponent>(transform);
	entity.AddComponent<ecs::BoundComponent>(bound);

	// Add entity to level map
	m_EntityMap[guid] = entity;

	return entity;
}

sad::ecs::Entity sad::Level::ImportEntityFromHandle(entt::entity handle)
{
	ecs::Entity entity = ecs::Entity(handle);
	core::Guid guid;

	// Retrieve GUID from entity being imported if it exists (required for re-importing/stopping levels)
	// Add GUID components if they don't exist on entities (required for adding components in levels)
	// This acts more like a failsafe, since all entities are provided a GUID if they are imported without one
	if (entity.HasComponent<ecs::GuidComponent>())
		guid = entity.GetComponent<ecs::GuidComponent>().m_GUID;
	else 
		guid = entity.AddComponent<ecs::GuidComponent>(core::Guid::CreateGuid()).m_GUID;

	// Always append entity GUIDs to the map on import
	m_EntityMap[guid] = entity;

	return entity;
}

bool sad::Level::DestroyEntity(sad::ecs::Entity entity)
{
	if (m_EntityMap.find(entity.GetGuid()) == m_EntityMap.end())
		return false;

	// Check if the entity had a SadBehaviour script
	// All SadBehaviours must be cleaned before entities are destroyed
	if (entity.HasComponent<ecs::ScriptComponent>() || entity.HasComponent<ecs::RuntimeScriptComponent>())
		sad::cs::ScriptingEngine::DestroySadBehaviourInstance(entity);

	// Unbind the entity from the level's entity map
	m_EntityMap.erase(entity.GetGuid());

	// Remove the entity from the registry
	sad::ecs::Registry::EraseEntityHandle(entity);

	return true;
}

bool sad::Level::DestroyEntityByName(const std::string& name)
{
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	ecs::Entity target = GetEntityByName(name);
	if (!target)
		return false;

	return DestroyEntity(target);
}

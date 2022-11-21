#include "sadpch.h"

#include "ScriptingSystem.h"

#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/ScriptComponent.h>

void pog::ecs::ScriptingSystem::Awake(sad::ecs::EntityWorld& world)
{
	auto view = world.view<sad::ecs::ScriptComponent>();
	for (auto handle : view)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// Scripting engine should handle instantiation of script methods
		// Also should handle validating that the provdied script still exists on the entity
		// tldr: this is what calls Awake() on all SadBehaviours
		sad::cs::ScriptingEngine::AwakeSadBehaviourInstance(entity);
	}
}

void pog::ecs::ScriptingSystem::Update(sad::ecs::EntityWorld& world)
{
	auto view = world.view<sad::ecs::ScriptComponent>();
	for (auto handle : view)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// tldr: this is what calls Update() on all SadBehaviours
		sad::cs::ScriptingEngine::UpdateSadBehaviourInstance(entity);
	}
}
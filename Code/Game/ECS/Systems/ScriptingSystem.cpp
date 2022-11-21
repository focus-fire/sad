#include "sadpch.h"

#include "ScriptingSystem.h"

#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/ScriptComponent.h>

void pog::ecs::ScriptingSystem::Awake(sad::ecs::EntityWorld& world)
{
	// Ensure that all script components added as native (or serialized) scripts are awake
	auto nativeScriptView = world.view<sad::ecs::ScriptComponent>();
	for (auto handle : nativeScriptView)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// Scripting engine should handle instantiation of script methods
		// Also should handle validating that the provdied script still exists on the entity
		// tldr: this is what calls Awake() on all SadBehaviours
		sad::cs::ScriptingEngine::AwakeNativeSadBehaviourInstance(entity);
	}

	// Ensure that all script components added during the runtime become awake
	auto runtimeScriptView = world.view<sad::ecs::RuntimeScriptComponent>();
	for (auto handle : runtimeScriptView)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// Scripting engine should handle instantiation of script methods
		// Also should handle validating that the provdied script still exists on the entity
		// tldr: this is what calls Awake() on all SadBehaviours that are created at runtime
		sad::cs::ScriptingEngine::AwakeRuntimeSadBehaviourInstance(entity);
	}
}

void pog::ecs::ScriptingSystem::Update(sad::ecs::EntityWorld& world)
{
	auto nativeScriptView = world.view<sad::ecs::ScriptComponent>();
	for (auto handle : nativeScriptView)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// tldr: this is what calls Update() on all SadBehaviours
		sad::cs::ScriptingEngine::UpdateSadBehaviourInstance(entity);
	}

	auto runtimeScriptView = world.view<sad::ecs::RuntimeScriptComponent>();
	for (auto handle : runtimeScriptView)
	{
		sad::ecs::Entity entity = sad::ecs::Entity(handle);

		// tldr: this is what calls Update() on all SadBehaviours created at runtime
		sad::cs::ScriptingEngine::UpdateSadBehaviourInstance(entity);
	}
}
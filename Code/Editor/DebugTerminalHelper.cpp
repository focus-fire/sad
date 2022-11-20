#include "sadpch.h"

#include "DebugTerminalHelper.h"

#include <Engine/Level.h>
#include <Engine/ResourceManager.h>
#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/ComponentTypes.h>

cap::DebugTerminalHelper::DebugTerminalHelper()
{
	add_command_({ "debug", "Log text to the debug sinks", Debug, NoCompletion });
	add_command_({ "clear", "Clear the terminal", Clear, NoCompletion });
	add_command_({ "close", "Closes the terminal", Close, NoCompletion });

	// Entity Commands
	add_command_({ "instantiate", "Instantiates an entity with a name and default resource", Instantiate, NoCompletion });
	add_command_({ "destroy", "Destroys an entity in the level with a name", Destroy, NoCompletion });
	add_command_({ "bind_script", "Binds a script to an entity in the level", BindScriptToEntity, NoCompletion });
	add_command_({ "unbind_script", "Unbinds a script to an entity in the level", UnbindScriptFromEntity, NoCompletion });
}

std::vector<std::string> cap::DebugTerminalHelper::NoCompletion(argument_type& arg) { return { " " }; }

void cap::DebugTerminalHelper::Clear(argument_type& arg)
{
	arg.term.clear();
}

void cap::DebugTerminalHelper::Debug(argument_type& arg)
{
	if (arg.command_line.size() < 2)
		return;

	std::string message = std::move(arg.command_line[1]);
	core::Log(ELogType::Debug, "[Terminal] {}", message);
}

void cap::DebugTerminalHelper::Close(argument_type& arg)
{
	arg.term.set_should_close();
}

void cap::DebugTerminalHelper::Instantiate(argument_type& arg)
{
	if (arg.command_line.size() < 2)
	{
		core::Log(ELogType::Error, "[Terminal] usage: instantiate <entity_name>");
		return;
	}

	std::string name = std::move(arg.command_line[1]);

	// TODO: Remove once models are implemented
	//		 Once models are implemented, we might want to make an 'add_comp' command that adds a component matching a string
	//		 ex: add_comp EntityA RenderableResource Cowboy.fbx
	sad::RenderableResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);
	sad::ecs::RenderableResourceComponent renderableResourceComponent;
	sad::Resource::ResourceData cubeData = { "FakeCube.test", "FakeCube.test", "FakeCube.test"};
	renderableResourceComponent.m_RenderableResource = core::CreatePointer<sad::RenderableResource>(cubeData, std::move(CubeGeometry));
	renderableResourceComponent.m_IsResourceDirty = true;

	// TODO: Is there a less scuffed way to retrieve the current level?
	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->InstantiateEntity(name);
	entity.AddComponent<sad::ecs::RenderableResourceComponent>(renderableResourceComponent);
}

void cap::DebugTerminalHelper::Destroy(argument_type& arg)
{
	if (arg.command_line.size() < 2)
	{
		core::Log(ELogType::Error, "[Terminal] usage: destroy <entity_name>");
		return;
	}
	
	std::string entityName = std::move(arg.command_line[1]);

	bool result = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->DestroyEntityByName(entityName);
	
	if (result)
	{
		core::Log(ELogType::Info, "[Terminal] Successfully destroyed entity {}", entityName);
	}
	else
	{
		core::Log(ELogType::Warn, "[Terminal] Failed to destroy entity {}, does it exist in the level?", entityName);
	}
}

void cap::DebugTerminalHelper::BindScriptToEntity(argument_type& arg)
{
	if (arg.command_line.size() < 3)
	{
		core::Log(ELogType::Error, "[Terminal] usage: bind_script <entity_name> <script_name>");
		return;
	}

	std::string entityName = std::move(arg.command_line[1]);
	std::string scriptName = std::move(arg.command_line[2]);

	// Check if entity exists
	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->GetEntityByName(entityName);
	if (!entity)
	{
		core::Log(ELogType::Error, "[Terminal] The entity {} doesn't exist in the level", entityName);
		return;
	}

	// Check if script exists
	if (!sad::cs::ScriptingEngine::SadBehaviourExists(scriptName))
	{
		core::Log(ELogType::Error, "[Terminal] The script {} doesn't exist in the currently loaded project assembly!", scriptName);
		return;
	}

	// Check if entity already has a script component
	if (entity.HasComponent<sad::ecs::ScriptComponent>())
	{
		core::Log(ELogType::Warn, "[Terminal] {} already has a script component", entityName);
		return;
	}

	entity.AddComponent<sad::ecs::ScriptComponent>(scriptName);
	core::Log(ELogType::Info, "[Terminal] Succesfully added {} to {}", scriptName, entityName);
}

void cap::DebugTerminalHelper::UnbindScriptFromEntity(argument_type& arg)
{
	if (arg.command_line.size() < 3)
	{
		core::Log(ELogType::Error, "[Terminal] usage: unbind_script <entity_name> <script_name>");
		return;
	}

	std::string entityName = std::move(arg.command_line[1]);
	std::string scriptName = std::move(arg.command_line[2]);

	// Check if entity exists
	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->GetEntityByName(entityName);
	if (!entity)
	{
		core::Log(ELogType::Error, "[Terminal] The entity {} doesn't exist in the level", entityName);
		return;
	}

	// Check if script exists
	bool scriptExistsInEngine = sad::cs::ScriptingEngine::SadBehaviourExists(scriptName);
	if (!scriptExistsInEngine)
	{
		core::Log(ELogType::Error, "[Terminal] The script {} doesn't exist in the currently loaded project assembly", scriptName);
		return;
	}

	// Check if instance of this specific script is instantiated on the entity
	bool instanceExistsOnEntity = sad::cs::ScriptingEngine::SadBehaviourInstanceExists(entity.GetGuid(), scriptName);
	if (!instanceExistsOnEntity)
	{
		core::Log(ELogType::Error, "[Terminal] {} doesn't have an instance of {}", entityName, scriptName);
		return;
	}

	// Check if entity doesn't have a script component to remove
	if (!entity.HasComponent<sad::ecs::ScriptComponent>())
	{
		core::Log(ELogType::Warn, "[Terminal] {} doesn't have a script component to unbind", entityName);
		return;
	}
	
	// Remove active script instance if script is detached from entity
	sad::cs::ScriptingEngine::DestroySadBehaviourInstance(entity);
	
	// Remove script component
	entity.RemoveComponent<sad::ecs::ScriptComponent>();
	core::Log(ELogType::Info, "[Terminal] Succesfully removed {} from {}", scriptName, entityName);
}

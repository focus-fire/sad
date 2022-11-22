#include "sadpch.h"

#include "DebugTerminalHelper.h"

#include <Engine/Level.h>
#include <Engine/ResourceManager.h>
#include <Engine/RenderableModel.h>
#include <Engine/PointLight.h>
#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/ComponentTypes.h>

cap::DebugTerminalHelper::DebugTerminalHelper()
{
	add_command_({ "debug", "Log text to the debug sinks", Debug, NoCompletion });
	add_command_({ "clear", "Clear the terminal", Clear, NoCompletion });
	add_command_({ "close", "Closes the terminal", Close, NoCompletion });

	// Entity Commands
	add_command_({ "instantiate", "Instantiates an entity with a name", Instantiate, NoCompletion });
	add_command_({ "instantiate_model", "Instantiates an entity with a name and model", InstantiateModel, NoCompletion });
	add_command_({ "instantiate_shape", "Instantiates an entity with a name and shape", InstantiateShape, NoCompletion });
	add_command_({ "destroy", "Destroys an entity in the level with a name", Destroy, NoCompletion });
	add_command_({ "bind_script", "Binds a script to an entity in the level", BindScriptToEntity, NoCompletion });
	add_command_({ "unbind_script", "Unbinds a script to an entity in the level", UnbindScriptFromEntity, NoCompletion });
	// add_command_({ "add_light", "Adds a light into the scene", AddLight, NoCompletion });
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
	sad::cs::ScriptingEngine::GetCurrentLevelInstance()->InstantiateEntity(name);
}

void cap::DebugTerminalHelper::InstantiateModel(argument_type& arg)
{
	if (arg.command_line.size() < 3)
	{
		core::Log(ELogType::Error, "[Terminal] usage: instantiate_model <entity_name> <model_name>");
		return;
	}

	std::string name = std::move(arg.command_line[1]);
	std::string modelName = std::move(arg.command_line[2]);

	sad::ModelResource* resource = sad::ResourceManager::GetResource<sad::ModelResource>(modelName);
	if (!resource)
	{
		core::Log(ELogType::Error, "[Terminal] The model {} doesn't exist or hasn't been cached by the ResourceManager", modelName);
		return;
	}

	// Initialize component data
	sad::ecs::ModelResourceComponent resourceComponent;
	resourceComponent.m_Model = resource;
	resourceComponent.m_IsResourceDirty = true;

	// Instantiate an entity and provide it the resource component
	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->InstantiateEntity(name);
	entity.AddComponent<sad::ecs::ModelResourceComponent>(resourceComponent);
}

void cap::DebugTerminalHelper::InstantiateShape(argument_type& arg)
{
	if (arg.command_line.size() < 3)
	{
		core::Log(ELogType::Error, "[Terminal] usage: instantiate_shape <entity_name> <shape>");
		return;
	}

	std::string name = std::move(arg.command_line[1]);
	std::string shapeName = std::move(arg.command_line[2]);

	if (shapeName != "Cube")
	{
		core::Log(ELogType::Error, "[Terminal] The shape {} doesn't exist as a primitive in the engine yet", shapeName);
		return;
	}

	// Initialize primitive data
	sad::PrimitiveResource::Geometry cubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);

	// Initialize component data
	sad::ecs::PrimitiveResourceComponent primitiveResourceComponent;
	primitiveResourceComponent.m_Primitive = core::CreatePointer<sad::PrimitiveResource>(std::move(cubeGeometry));
	primitiveResourceComponent.m_IsResourceDirty = true;

	// Instantiate an entity and provide it the resource component
	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->InstantiateEntity(name);
	entity.AddComponent<sad::ecs::PrimitiveResourceComponent>(primitiveResourceComponent);
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
	core::Log(ELogType::Info, "[Terminal] Succesfully removed {} from {}", scriptName, entityName);
}

void cap::DebugTerminalHelper::AddLight(argument_type& arg)
{
	if (arg.command_line.size() < 2)
	{
		core::Log(ELogType::Error, "[Terminal] usage: add_light <entity_name>");
		return;
	}

	std::string entityName = std::move(arg.command_line[1]);

	sad::ecs::Entity entity = sad::cs::ScriptingEngine::GetCurrentLevelInstance()->InstantiateEntity(entityName);
	sad::PointLight light = sad::PointLight();
	entity.AddComponent<sad::ecs::PointLightComponent>(light);
}

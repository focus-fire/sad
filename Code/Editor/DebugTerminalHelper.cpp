#include "sadpch.h"

#include "DebugTerminalHelper.h"

#include <Engine/ResourceManager.h>
#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/ComponentTypes.h>

cap::DebugTerminalHelper::DebugTerminalHelper()
{
	add_command_({ "debug", "Log text to the debug sinks", Debug, NoCompletion });
	add_command_({ "echo", "Echo text to the terminal", Echo, NoCompletion });
	add_command_({ "clear", "Clear the terminal", Clear, NoCompletion });
	add_command_({ "close", "Closes the terminal", Close, NoCompletion });

	// Entity Commands
	add_command_({ "instantiate", "Instantiates an entity with a name and default resource", Instantiate, NoCompletion });
	add_command_({ "destroy", "Destroys an entity in the level with a name", Destroy, NoCompletion });
}

std::vector<std::string> cap::DebugTerminalHelper::NoCompletion(argument_type& arg) { return {}; }

void cap::DebugTerminalHelper::Clear(argument_type& arg)
{
	arg.term.clear();
}

void cap::DebugTerminalHelper::Echo(argument_type& arg)
{
	if (arg.command_line.size() < 2)
		return;

	std::string str = std::move(arg.command_line[1]);
	for (auto it = std::next(arg.command_line.begin(), 2); it != arg.command_line.end(); it++)
	{
		str += " " + std::move(*it);
	}

	ImTerm::message message;
	message.value = std::move(str);
	message.color_beg = message.color_end = 0;
	arg.term.add_message(std::move(message));
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
		core::Log(ELogType::Error, "[Terminal] Please enter the name of an entity when using the instantiate command");
		return;
	}

	std::string name = std::move(arg.command_line[1]);

	// TODO: Replace with resource manager call to model file?
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
		core::Log(ELogType::Error, "[Terminal] Please enter the name of an entity to destroy when using the destroy command");
		return;
	}
	
	std::string name = std::move(arg.command_line[1]);
	sad::cs::ScriptingEngine::GetCurrentLevelInstance()->DestroyEntityByName(name);
}
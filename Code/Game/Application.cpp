#include "sadpch.h"

#include "Application.h"

#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Systems/RenderableObjectSystem.h>
#include <Engine/ECS/Components/ComponentTypes.h>

#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/Renderer/VertexArray.h>
#include <Engine/Renderer/IndexBuffer.h>
#include <Engine/Renderer/FrameBuffer.h>
#include <Engine/Renderer/ShaderResource.h>
#include <Engine/Renderer/RenderBuddy.h>

#include <Engine/AudioManager.h>
#include <Engine/ResourceManager.h>
#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>

#include "ECS/Systems/ScriptingSystem.h"

pog::Application::Application()
	: sad::Application()
{
	// Game is initialized here
}

pog::Application::~Application()
{
	// Game is destroyed here
}

void pog::Application::Start()
{
	// Awaken scripts
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	ecs::ScriptingSystem::Awake(world);
}

void pog::Application::Update(float dt)
{
	std::lock_guard lock = std::lock_guard<std::mutex>(m_GameMutex);

	// Update scripts
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	ecs::ScriptingSystem::Update(world);
}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}

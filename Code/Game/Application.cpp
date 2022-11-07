#include "sadpch.h"

#include "Application.h"

#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Systems/RenderableObjectSystem.h>
#include <Engine/ECS/Components/RenderableResourceComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/ControllerComponent.h>

#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/Renderer/VertexArray.h>
#include <Engine/Renderer/IndexBuffer.h>
#include <Engine/Renderer/FrameBuffer.h>
#include <Engine/Renderer/ShaderResource.h>

#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>
#include <Engine/JsonManager.h>

/**
 * @brief 
 * @sample
 * Go to Docs folder and use ApplicationFileDefault.md to put back the test case
*/
pog::Application::Application()
	: sad::Application()
{}

pog::Application::~Application()
{
	// Game is destroyed here
}

void pog::Application::Start()
{}

void pog::Application::Update(float dt)
{}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}

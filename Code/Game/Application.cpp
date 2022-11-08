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
{
	std::lock_guard g = std::lock_guard<std::mutex>(m_GameMutex);

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	m_CubeTranslate += 1.0f * dt;
	if (m_CubeTranslate >= glm::pi<float>())
		m_CubeTranslate = -1.0f * glm::pi<float>();

	// Manipulate first entity transform
	m_FirstCubeEntity.Transform.Rotate(glm::vec3(10.0f * dt));
	m_FirstCubeEntity.Transform.Translate(glm::vec3(0.0f, glm::sin(m_CubeTranslate) * dt, 0.0f));

	// Manipulate second entity transform
	m_SecondCubeEntity.Transform.Rotate(glm::vec3(10.0f * dt));
	m_SecondCubeEntity.Transform.Translate(glm::vec3(glm::sin(-m_CubeTranslate * 2) * dt, 0.0f, 0.0f));
}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}

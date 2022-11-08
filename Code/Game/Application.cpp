#include "sadpch.h"

#include "Application.h"

#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Systems/RenderableObjectSystem.h>
#include <Engine/ECS/Components/RenderableResourceComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/ControllerComponent.h>
#include <Engine/ECS/Components/BoundComponent.h>

#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/Renderer/VertexArray.h>
#include <Engine/Renderer/IndexBuffer.h>
#include <Engine/Renderer/FrameBuffer.h>
#include <Engine/Renderer/ShaderResource.h>
#include <Engine/Renderer/RenderBuddy.h>

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
	//, m_CubeGeometry(sad::RenderableResource::Geometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount))
{
	//m_CubeResource = new sad::RenderableResource({ "TestCube.fake", "TestCube.fake" }, m_CubeGeometry);
}

pog::Application::~Application()
{
	// Game is destroyed here
	//delete m_CubeResource;
}

void pog::Application::Start()
{
	// Add resource and transform components to the entities
	//m_FirstCubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource });
	//m_FirstCubeEntity.AddComponent<sad::ecs::TransformComponent>({ m_FirstCubeEntity.Transform });
	//m_FirstCubeEntity.AddComponent<sad::ecs::BoundComponent>({ m_FirstCubeEntity.Bound });

	//m_FirstCubeEntity.AddEmptyComponent<sad::ecs::PlayerControllerComponent>({});

	//m_SecondCubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource });
	//m_SecondCubeEntity.AddComponent<sad::ecs::TransformComponent>({ m_SecondCubeEntity.Transform });
	//m_SecondCubeEntity.AddComponent<sad::ecs::BoundComponent>({ m_SecondCubeEntity.Bound });

	//m_ThirdPlaneEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource });
	//m_ThirdPlaneEntity.AddComponent<sad::ecs::TransformComponent>({ m_ThirdPlaneEntity.Transform });

	// Translation Logic (-pi to pi for demo)
	m_CubeTranslate = -1.0f * glm::pi<float>();
	m_LastTime = std::chrono::steady_clock::now();
}

void pog::Application::Update(float dt)
{
	std::lock_guard lock = std::lock_guard<std::mutex>(m_GameMutex);

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	//if (m_FirstCubeEntity.Bound->Intersects(*m_SecondCubeEntity.Bound.get()) && m_LogLimit <= 50)
	//{
	//	glm::vec3 firstBoundPos = m_FirstCubeEntity.Bound->GetPosition();
	//	core::Log(ELogType::Debug, "Collision detected @ {}, {}, {}", firstBoundPos.x, firstBoundPos.y, firstBoundPos.z);
	//	m_LogLimit++;
	//}

	// Sample 'Script' to rotate objects
	auto view = sad::ecs::Registry::GetEntityWorld().view<const sad::ecs::TransformComponent, const sad::ecs::BoundComponent, const sad::ecs::RenderableObjectComponent>();
	for (auto [entity, transformComponent, boundComponent, renderableComponent] : view.each())
	{
		sad::Transform* transform = transformComponent.m_Transform.get();
		transform->Rotate(glm::vec3(10.0f * dt));
		transform->Translate(glm::vec3(glm::sin(-m_CubeTranslate * 2) * dt, 0.0f, 0.0f));
	}
}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}

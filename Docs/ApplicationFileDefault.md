/////////Application.h/////////

#pragma once

#include <Engine/Application.h>
#include <Engine/RenderableResource.h>

namespace pog
{
	class Application final : public sad::Application
	{
	public:
		explicit Application();
		~Application() override;

		void Start() override;

		void Update(float dt) override;
		void Teardown() override;
	
	private:
		// Temporary Variables for Test Level
		sad::RenderableResource* m_CubeResource;
		sad::RenderableResource::Geometry m_CubeGeometry;
		sad::ecs::Entity m_FirstCubeEntity;
		sad::ecs::Entity m_SecondCubeEntity;
		float m_CubeTranslate;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
	};
}

====================================================================================


////////Application.cpp//////////

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

pog::Application::Application()
	: sad::Application(),
	m_CubeGeometry(sad::RenderableResource::Geometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount))
{
	// TODO: Solve IResource constructor dilemma
	m_CubeResource = new sad::RenderableResource({ "TestCube.fake", "TestCube.fake" }, m_CubeGeometry);
}

pog::Application::~Application()
{
	// Game is destroyed here
	delete m_CubeResource;
}

void pog::Application::Start()
{
	// Add resource and transform components to the entities
	m_FirstCubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource });

	core::Pointer<sad::Transform> firstTransform = core::CreatePointer<sad::Transform>(m_FirstCubeEntity.Transform);
	m_FirstCubeEntity.AddComponent<sad::ecs::TransformComponent>({ firstTransform });
	m_FirstCubeEntity.AddEmptyComponent<sad::ecs::PlayerControllerComponent>({});

	core::Pointer<sad::Transform> secondTransform = core::CreatePointer<sad::Transform>(m_SecondCubeEntity.Transform);
	m_SecondCubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource });
	m_SecondCubeEntity.AddComponent<sad::ecs::TransformComponent>({ secondTransform });
	
	// Translation Logic (-pi to pi for demo)
	m_CubeTranslate = -1.0f * glm::pi<float>();
	m_LastTime = std::chrono::steady_clock::now();
}

void pog::Application::Update(float dt)
{
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

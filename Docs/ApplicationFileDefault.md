# Level Defaults
The following code block contains a sample used to render the Milestone 1 level.

### Sample Game/Application.cpp

```
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

#include <Engine/AudioManager.h>
#include <Engine/ResourceManager.h>
#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>
#include <Engine/JsonManager.h>

pog::Application::Application()
	: sad::Application()
	, m_RenderableGeometry(sad::RenderableResource::Geometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount))
{
	// Game is initialized here
	sad::Resource::ResourceData data = { "TestCube.fake", "TestCube.fake", "TestCube.fake" };
	m_CubeResource = core::CreatePointer<sad::RenderableResource>(data, m_RenderableGeometry);
}

pog::Application::~Application()
{
	// Game is destroyed here
}

void pog::Application::Start()
{
	m_CollisionSoundEffect = sad::ResourceManager::GetResource<sad::AudioResource>("jump.wav");

	m_First.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource, true });
	m_Second.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource, true });
	m_Third.AddComponent<sad::ecs::RenderableResourceComponent>({ m_CubeResource, true });

	m_First.AddComponent<sad::ecs::TransformComponent>({ m_First.Transform });
	m_Second.AddComponent<sad::ecs::TransformComponent>({ m_Second.Transform });
	m_Third.AddComponent<sad::ecs::TransformComponent>({ m_Third.Transform });

	m_First.AddComponent<sad::ecs::BoundComponent>({ m_First.Bound });
	m_Second.AddComponent<sad::ecs::BoundComponent>({ m_Second.Bound });

	m_First.AddEmptyComponent<sad::ecs::PlayerControllerComponent>({});

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

	// Sample 'Script' to rotate objects
	auto view = sad::ecs::Registry::GetEntityWorld().view<const sad::ecs::TransformComponent, const sad::ecs::BoundComponent, const sad::ecs::RenderableObjectComponent>();
	for (auto [entity, transformComponent, boundComponent, renderableComponent] : view.each())
	{
		sad::Bound* bound = boundComponent.m_Bound.get();
		sad::Transform* transform = transformComponent.m_Transform.get();
		transform->Rotate(glm::vec3(10.0f * dt));
		transform->Translate(glm::vec3(glm::sin(-m_CubeTranslate * 2) * dt, 0.0f, 0.0f));

		for (auto [e, t, b, r] : view.each())
		{
			if (entity != e)
			{
				sad::Bound* bound2 = b.m_Bound.get();
				if (bound->Intersects(*bound2))
				{
					sad::AudioManager::PlaySFX(m_CollisionSoundEffect);
				}
			}
		}
	}
}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}
```

### Sample Game/Application.h

```
#pragma once

#include <Core/Memory.h>

#include <Engine/Application.h>
#include <Engine/RenderableResource.h>
#include <Engine/AudioResource.h>

#include <Engine/ECS/Entity.h>

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
		std::mutex m_GameMutex;

		float m_CubeTranslate;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;

		sad::RenderableResource::Geometry m_RenderableGeometry;
		core::Pointer<sad::RenderableResource> m_CubeResource;

		sad::ecs::Entity m_First;
		sad::ecs::Entity m_Second;
		sad::ecs::Entity m_Third;

		sad::AudioResource* m_CollisionSoundEffect;
	};
}
```

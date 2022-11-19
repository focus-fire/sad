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
	m_CollisionSoundEffect = sad::ResourceManager::GetResource<sad::AudioResource>("jump.wav");

	// Translation Logic (-pi to pi for demo)
	m_CubeTranslate = -1.0f * glm::pi<float>();
	m_LastTime = std::chrono::steady_clock::now();

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

	// Sample 'Script' to rotate objects
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	auto oldView = world.view<const sad::ecs::TransformComponent, const sad::ecs::BoundComponent, const sad::ecs::RenderableObjectComponent>();
	for (auto [entity, transformComponent, boundComponent, renderableComponent] : oldView.each())
	{
		sad::Bound* bound = boundComponent.m_Bound.get();
		sad::Transform* transform = transformComponent.m_Transform.get();
		transform->Rotate(glm::vec3(10.0f * dt));
		transform->Translate(glm::vec3(glm::sin(-m_CubeTranslate * 2) * dt, 0.0f, 0.0f));

		for (auto [secondEntity, secondTransform, secondBound, secondRenderable] : oldView.each())
		{
			if (entity != secondEntity)
			{
				sad::Bound* bound2 = secondBound.m_Bound.get();
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

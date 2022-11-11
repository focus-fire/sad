#include "sadpch.h"

#include "BoundSystem.h"

#include <entt/entt.hpp>

#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/BoundComponent.h>

#include <Engine/Renderer/RenderBuddy.h>

void sad::ecs::BoundSystem::Update(EntityWorld& world)
{
	auto view = world.view<const ecs::TransformComponent, ecs::BoundComponent>();
	for (auto [entity, transformComponent, boundComponent] : view.each())
	{
		Transform* transform = transformComponent.m_Transform.get();
		Bound* bound = boundComponent.m_Bound.get();

		// TODO: Only perform a recalculation on a bound when it's transform moves
		bound->SetPosition(transform->GetPosition());
		bound->SetScale(transform->GetScale());
		bound->CalculateBound();
	}
}
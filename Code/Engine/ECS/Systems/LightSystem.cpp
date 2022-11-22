#include "sadpch.h"

#include "LightSystem.h"

#include <Engine/ECS/Components/ComponentTypes.h>

void sad::ecs::LightSystem::Update(EntityWorld& world)
{
	auto view = world.view<ecs::PointLightComponent, ecs::TransformComponent>();
	for (auto [handle, pointLight, transformComponent] : view.each())
	{
		Transform* transform = transformComponent.m_Transform.get();
		pointLight.m_Light.Position = transform->GetPosition();
	}
}
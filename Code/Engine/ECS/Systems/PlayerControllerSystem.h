#pragma once

#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/InputManager.h>

namespace sad::ecs
{
	class PlayerControllerSystem
	{
	public:
		static void Update(EntityWorld& world);

	private:
		static void PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
		static void EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
	};

}
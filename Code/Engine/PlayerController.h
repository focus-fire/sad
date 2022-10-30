#pragma once

#include "ECS/Registry.h"
#include "ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"

namespace sad
{
	class PlayerController
	{
	public:
		PlayerController();
		~PlayerController();

		static void Update();

	private:
		static void PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
		static void EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
	};

}
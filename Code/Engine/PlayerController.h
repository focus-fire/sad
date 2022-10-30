#pragma once

#include "ECS/Registry.h"
#include "ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"

namespace sad
{
	enum class ControlMode
	{
		EditorMode,
		PlayerMode
	};

	class PlayerController
	{
	public:
		PlayerController();
		~PlayerController();

		static void Update();
		static void ChangeModes(ControlMode mode);

	private:
		static void PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
		static void EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
	};

}
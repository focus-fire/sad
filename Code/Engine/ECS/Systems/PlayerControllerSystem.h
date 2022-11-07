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
		/**
		 * @brief Handles movement to run on player controller components.
		 * @param input inputmanager
		 * @param transformComponent 
		 * @param moveSpeedMultiplier 
		*/
		static void PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
		/**
		 * @brief Handles movement to run on editor controller components.
		 * @param input inputmanager
		 * @param transformComponent
		 * @param moveSpeedMultiplier
		*/
		static void EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed);
	};

}
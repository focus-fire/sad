#pragma once

#include <Engine/ECS/Systems/PlayerControllerSystem.h>

namespace sad::ecs
{
	/**
	 * @brief Component to mark entity for player movement and controls
	*/
	struct PlayerControllerComponent{};

	/**
	 * @brief Component to mark entity for editor movement and controls
	*/
	struct EditorControllerComponent {};
}

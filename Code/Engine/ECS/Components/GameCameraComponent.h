#pragma once

#include <Engine/GameCamera.h>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the GameCamera of a particular sad::Entity
	 *
	 * @note As most TransformComponents should be manipulated by entities themselves, there
	 *       shouldn't be a corresponding TransformSystem or other systems attempting to modify them.
	*/
	struct GameCameraComponent
	{
		GameCamera* m_Camera;
	};
}
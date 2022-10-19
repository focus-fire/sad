#pragma once

#include <Engine/PlayerController.h>

namespace sad::ecs
{
	/**
	 * @brief Contains reference to a PlayerController
	*/
	struct PlayerControllerComponent
	{
		PlayerController* m_PlayerController;
	};
}

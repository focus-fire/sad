#pragma once

#include <Engine/PlayerController.h>

namespace sad::ecs
{
	struct PlayerControllerComponent
	{
		PlayerController* m_PlayerController;

		PlayerControllerComponent(PlayerController* p) 
		{
			m_PlayerController = p;
		}
	};
}

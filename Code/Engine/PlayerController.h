#pragma once

#include "ECS/Registry.h"

namespace sad
{
	class PlayerController
	{
	public:
		PlayerController();
		~PlayerController();

		void Update(sad::ecs::EntityWorld& world);
	};
}
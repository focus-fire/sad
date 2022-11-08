#pragma once

#include <Engine/ECS/Registry.h>

namespace sad::ecs
{
	class BoundSystem
	{
	public:
		static void Update(EntityWorld& world);
	};
}

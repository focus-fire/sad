#pragma once

#include <Engine/ECS/Registry.h>

namespace sad::ecs
{
	class LightSystem
	{
	public:
		static void Update(EntityWorld& world);
	};
}

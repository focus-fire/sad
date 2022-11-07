#pragma once

#include <Engine/Transform.h>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the Transform of a particular sad::Entity
	 *
	 * @note As most TransformComponents should be manipulated by entities themselves, there
	 *       shouldn't be a corresponding TransformSystem or other systems attempting to modify them.
	*/
	struct TransformComponent
	{
		Transform* m_Transform;
	};
}

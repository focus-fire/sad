#pragma once

#include <Engine/Bound.h>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the Bound of a particular sad::Entity
	 *
	 * @note As most BoundComponents should be manipulated by entities themselves, there
	 *       shouldn't be a corresponding BoundSystem or other systems attempting to modify them.
	*/
	struct BoundComponent
	{
		core::Pointer<Bound> m_Bound;
	};
}
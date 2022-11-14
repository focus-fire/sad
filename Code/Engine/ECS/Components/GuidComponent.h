#pragma once

#include <Core/Guid.h>

namespace sad::ecs
{
	/**
	 * @brief Component that creates a GUID upon construction, used to identify entities
	*/
	struct GuidComponent
	{
		core::Guid m_GUID;
	};
}

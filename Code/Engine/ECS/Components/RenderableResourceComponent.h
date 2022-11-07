#pragma once

#include <Engine/RenderableResource.h>

namespace sad::ecs
{
	/**
	 * @brief Component containing a reference for a particular RenderableResource
	*/
	struct RenderableResourceComponent
	{
		RenderableResource* m_RenderableResource;
		bool m_IsResourceDirty;

		RenderableResourceComponent(RenderableResource* renderableResource)
			: m_RenderableResource(renderableResource)
			, m_IsResourceDirty(true)
		{ }
	};
}

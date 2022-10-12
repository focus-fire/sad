#pragma once

#include <memory>

#include <Engine/RenderableObject.h>

namespace sad::ecs
{
	/**
	 * @brief Contains a shared reference for a particular RenderableObject
	*/
	struct RenderableObjectComponent
	{
		std::shared_ptr<RenderableObject> m_RenderableObject;
	};
}

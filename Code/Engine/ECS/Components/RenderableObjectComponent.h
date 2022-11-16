#pragma once

#include <Core/Memory.h>

#include <Engine/RenderableObject.h>

namespace sad::ecs
{
	/**
	 * @brief Contains a pointer for a RenderableObject
	 * @note RenderableObjects contain the actual geometric data for a renderable in the engine
	*/
	struct RenderableObjectComponent
	{
		core::Pointer<RenderableObject> m_RenderableObject;
	};
}

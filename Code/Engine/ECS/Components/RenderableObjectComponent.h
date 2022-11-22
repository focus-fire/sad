#pragma once

#include <Core/Memory.h>

#include <Engine/RenderableModel.h>
#include <Engine/RenderablePrimitive.h>

namespace sad::ecs
{
	/**
	 * @brief Tag component that marks all renderable objects
	*/
	struct RenderableObjectComponent
	{ };

	/**
	 * @brief Contains a pointer for a RenderableModel
	*/
	struct RenderableModelComponent
	{
		RenderableModel m_RenderableModel;
	};

	/**
	 * @brief Contains a pointer for a RenderablePrimitive
	*/
	struct RenderablePrimitiveComponent
	{
		core::Pointer<RenderablePrimitive> m_Primitive;
	};
}

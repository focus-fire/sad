#pragma once

#include <nlohmann/json.hpp>

#include <Engine/RenderableResource.h>
#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/Resource.h>

namespace sad::ecs
{
	/**
	 * @brief Contains a reference for a particular RenderableResourceComponent 
	*/
	struct RenderableResourceComponent
	{
		ModelResource* m_Renderable;
		bool m_IsResourceDirty;
	};

	/**
	 * @brief Component containing a reference for a particular PrimitiveResource 
	 *
	 * @note Most PrimitiveResourceComponent's should be initialized with an m_IsResourceDirty of 'true'.
	 *		 This ensures that they get absolved into a RenderablePrimitive in the next frame.
	*/
	struct PrimitiveResourceComponent
	{
		core::Pointer<PrimitiveResource> m_Primitive;
		bool m_IsResourceDirty;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::PrimitiveResourceComponent& primitiveResource)
	{
		JSON =
		{
			{ "PrimitiveType", "Cube" },
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::PrimitiveResourceComponent& primitiveResource)
	{
		PrimitiveResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);
		primitiveResource.m_Primitive = core::CreatePointer<PrimitiveResource>(std::move(CubeGeometry));
		primitiveResource.m_IsResourceDirty = true;
	}
}

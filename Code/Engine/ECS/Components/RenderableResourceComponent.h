#pragma once

#include <nlohmann/json.hpp>

#include <Engine/RenderableResource.h>
#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/Resource.h>

namespace sad::ecs
{
	/**
	 * @brief Component containing a reference for a particular RenderableResource
	 *
	 * @note Most RenderableResourceComponent's should be initialized with an m_IsResourceDirty of 'true'.
	 *		 This ensures that they get absolved into a RenderableObject in the next frame.
	*/
	struct RenderableResourceComponent
	{
		core::Pointer<RenderableResource> m_RenderableResource;
		bool m_IsResourceDirty;
	};

	struct ModelResourceComponent
	{
		RenderableResource* m_ModelResource;
		bool m_IsResourceDirty;
	};

	//TODO: Temporary Filler for serialization, fill in when 3d modelling is done
	inline void to_json(nlohmann::json& JSON, const sad::ecs::RenderableResourceComponent& renderableResource)
	{
		JSON =
		{
			{ "GUID", renderableResource.m_RenderableResource->GetResourceGuid().ToString() },
			{ "FileName", renderableResource.m_RenderableResource->GetResourceFileName() },
			{ "DataPath", renderableResource.m_RenderableResource->GetResourceDataPath() },
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::RenderableResourceComponent& renderableResource)
	{
		RenderableResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);
		// TODO: Remove IReource requirement... 
		Resource::ResourceData modelData = { JSON["FileName"],JSON["DataPath"], JSON["DataPath"] };

		renderableResource.m_RenderableResource = core::CreatePointer<RenderableResource>(modelData, std::move(CubeGeometry));
		renderableResource.m_IsResourceDirty = true;
	}
}

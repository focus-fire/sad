<<<<<<< HEAD
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
=======
#pragma once

#include <nlohmann/json.hpp>

#include <Engine/RenderableResource.h>
#include <Engine/Renderer/Sample/Cube.h>
#include <Engine/IResource.h>

namespace sad::ecs
{
	/**
	 * @brief Component containing a reference for a particular RenderableResource
	*/
	struct RenderableResourceComponent
	{
		core::Pointer<RenderableResource> m_RenderableResource;
		bool m_IsResourceDirty;

		RenderableResourceComponent() { }

		RenderableResourceComponent(RenderableResource* renderableResource)
			: m_RenderableResource(renderableResource)
			, m_IsResourceDirty(true)
		{ }
	};

	
	//TODO:Temporary Filler for serialization, fill in when 3d modelling is done
	inline void to_json(nlohmann::json& JSON, const sad::ecs::RenderableResourceComponent& renderableResource)
	{
		JSON =
		{
			{ "GUID", renderableResource.m_RenderableResource->GetResourceId().ToString() },
			{ "Name", renderableResource.m_RenderableResource->GetResourceName() },
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::RenderableResourceComponent& renderableResource)
	{
		RenderableResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);

		// TODO: Remove IReource requirement... 
		IResource::ResourceData cubeData = { "FakeCube.test", "FakeCube.test"};
		renderableResource.m_RenderableResource = core::CreatePointer<RenderableResource>(cubeData, std::move(CubeGeometry));
		renderableResource.m_IsResourceDirty = true;
	}
}
>>>>>>> dev

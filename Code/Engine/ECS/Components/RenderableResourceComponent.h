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
	inline void to_json(nlohmann::json& j, const sad::ecs::RenderableResourceComponent& p)
	{
		j =
		{
			{ "GUID", p.m_RenderableResource->GetResourceId().ToString() },
			{ "Name", p.m_RenderableResource->GetResourceName() },
		};
	}

	inline void from_json(const nlohmann::json& j, sad::ecs::RenderableResourceComponent& p)
	{
		RenderableResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);

		// TODO: Remove IReource requirement... 
		IResource::ResourceData cubeData = { "FakeCube.test", "FakeCube.test"};
		p.m_RenderableResource = core::CreatePointer<RenderableResource>(cubeData, std::move(CubeGeometry));
		p.m_IsResourceDirty = true;
	}
}

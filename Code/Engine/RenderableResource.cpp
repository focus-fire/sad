#include "sadpch.h"

#include "RenderableResource.h"

#include "ECS/Registry.h"

sad::RenderableResource::RenderableResource(const IResource::ResourceData& resourceData, const Geometry& geometry)
	: IResource(resourceData)
	, m_Geometry(geometry)
{ }


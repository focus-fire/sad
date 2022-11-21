#include "sadpch.h"

#include "RenderableResource.h"

#include "ECS/Registry.h"

sad::RenderableResource::RenderableResource()
	: Resource(Resource::Null())
{ }

sad::RenderableResource::RenderableResource(const Resource::ResourceData& resourceData)
	: Resource(resourceData)
{ }

sad::RenderableResource::~RenderableResource()
{ }

sad::RenderableResource::RenderableResource(const Resource::ResourceData& resourceData, const Geometry& geometry)
	: Resource(resourceData)
	, m_Geometry(geometry)
{ }


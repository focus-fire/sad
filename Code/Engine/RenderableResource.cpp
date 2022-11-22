#include "sadpch.h"

#include "RenderableResource.h"

#include "ECS/Registry.h"

sad::ModelResource::ModelResource()
	: Resource(Resource::Null())
{ }

sad::ModelResource::ModelResource(const Resource::ResourceData& resourceData)
	: Resource(resourceData)
{ }

sad::ModelResource::~ModelResource()
{ }

sad::PrimitiveResource::PrimitiveResource(const Geometry& geometry)
	: m_Geometry(geometry)
{ }


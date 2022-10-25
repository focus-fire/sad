#include "sadpch.h"

#include "RenderableResource.h"

#include "ECS/Registry.h"

sad::RenderableResource::RenderableResource(const Geometry& geometry)
	: m_Geometry(geometry)
{ }


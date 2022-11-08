#include "sadpch.h"

#include "RenderableObjectSystem.h"

#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/RenderableResourceComponent.h>

void sad::ecs::RenderableObjectSystem::Update(EntityWorld& world)
{ 
	// Create a RenderableObject whenever an entity has a RenderableResource
	auto view = world.view<RenderableResourceComponent>();
	for (auto [entity, res] : view.each())
	{
		// Skip updating the resource if it isn't dirty
		if (!res.m_IsResourceDirty)
			continue;
		
		core::Pointer<RenderableObject> renderable = core::CreatePointer<RenderableObject>(res.m_RenderableResource);
		SAD_ASSERT(renderable, "Failed to create RenderableObject from RenderableResource");
		world.emplace<RenderableObjectComponent>(entity, renderable);

		// Mark the RenderableObject as clean 
		res.m_IsResourceDirty = false;
	}
}

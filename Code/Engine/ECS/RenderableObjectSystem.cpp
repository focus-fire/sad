#include "sadpch.h"

#include "RenderableObjectSystem.h"

#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>

#include "Registry.h"

void sad::ecs::RenderableObjectSystem::Update()
{ 
	EntityWorld* world = Registry::GetEntityWorld();

	auto view = world->view<RenderableResourceComponent>();
	for (auto [entity, res] : view.each())
	{
		// Skip updating the resource if it isn't dirty
		if (!res.m_IsResourceDirty)
			continue;
		
		// Create a shared pointer to a RenderableObject here...
		// This has a bit of overhead due to reference counting... however!
		// 
		// Alternatives?
		// If we create a normal pointer with 'new' how and when do we 'delete' it?
		// 'unique_ptr' is non-copyable, so entt can't use it...
		// Passing by value could work... but this copies the object - thus killing and recreating some GL buffers in weird ways
		std::shared_ptr<RenderableObject> renderable = std::make_shared<RenderableObject>(res.m_RenderableResource);
		SAD_ASSERT(renderable, "Failed to create RenderableObject from RenderableResource");
		world->emplace<RenderableObjectComponent>(entity, renderable);

		// Mark the RenderableObject as clean 
		res.m_IsResourceDirty = false;
	}
}

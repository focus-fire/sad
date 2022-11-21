#include "sadpch.h"

#include "RenderableObjectSystem.h"

#include <Engine/RenderableResource.h>
#include <Engine/RenderableObject.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/RenderableResourceComponent.h>

void sad::ecs::RenderableObjectSystem::Update(EntityWorld& world)
{ 
	// Create a RenderableObject whenever an entity has a RenderableResource
	auto primitiveView = world.view<RenderableResourceComponent>();
	for (auto [handle, res] : primitiveView.each())
	{
		// Skip updating the resource if it isn't dirty
		if (!res.m_IsResourceDirty)
			continue;

		ecs::Entity entity = ecs::Entity(handle);
		
		core::Pointer<RenderableObject> renderable = core::CreatePointer<RenderableObject>(res.m_RenderableResource.get());
		SAD_ASSERT(renderable, "Failed to create RenderableObject from RenderableResource");
		entity.AddComponent<RenderableObjectComponent>(renderable);

		// Mark the RenderableObject as clean 
		res.m_IsResourceDirty = false;
	}

	auto modelView = world.view<ModelResourceComponent>();
	for (auto [handle, res] : modelView.each())
	{
		if (!res.m_IsResourceDirty)
			continue;

		ecs::Entity entity = ecs::Entity(handle);

		Model model = Model(res.m_ModelResource->GetResourceAbsolutePath());
		entity.AddComponent<RenderableModelComponent>(std::move(model));

		// Mark the RenderableObject as clean 
		res.m_IsResourceDirty = false;
	}
}

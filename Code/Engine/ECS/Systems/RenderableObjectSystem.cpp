#include "sadpch.h"

#include "RenderableObjectSystem.h"

#include <Engine/RenderableModel.h>
#include <Engine/RenderableResource.h>

#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/RenderableResourceComponent.h>

void sad::ecs::RenderableObjectSystem::Update(EntityWorld& world)
{ 
	CreateRenderableModels(world);

	CreateRenderablePrimitives(world);
}

void sad::ecs::RenderableObjectSystem::CreateRenderableModels(EntityWorld& world)
{
	auto view = world.view<RenderableResourceComponent>();
	for (auto [handle, renderableResourceComponent] : view.each())
	{
		if (!renderableResourceComponent.m_IsResourceDirty)
			continue;

		ecs::Entity entity = ecs::Entity(handle);

		// Import the model and its geometry, attach it to the entity and mark it as a renderable
		RenderableModel model = RenderableModel(renderableResourceComponent.m_Renderable->GetResourceAbsolutePath());
		entity.AddComponent<RenderableModelComponent>(std::move(model));
		entity.AddEmptyComponent<RenderableObjectComponent>({});

		// Mark the RenderablePrimitive as clean 
		renderableResourceComponent.m_IsResourceDirty = false;
	}
}

void sad::ecs::RenderableObjectSystem::CreateRenderablePrimitives(EntityWorld& world)
{
	auto view = world.view<PrimitiveResourceComponent>();
	for (auto [handle, primitiveResourceComponent] : view.each())
	{
		// Skip updating the resource if it isn't dirty
		if (!primitiveResourceComponent.m_IsResourceDirty)
			continue;

		ecs::Entity entity = ecs::Entity(handle);
		
		core::Pointer<RenderablePrimitive> renderable = core::CreatePointer<RenderablePrimitive>(primitiveResourceComponent.m_Primitive.get());
		SAD_ASSERT(renderable, "Failed to create RenderablePrimitive from ModelResource");
		entity.AddComponent<RenderablePrimitiveComponent>(renderable);
		entity.AddEmptyComponent<RenderableObjectComponent>({});

		// Mark the RenderablePrimitive as clean 
		primitiveResourceComponent.m_IsResourceDirty = false;
	}
}

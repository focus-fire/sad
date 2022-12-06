#include "sadpch.h"

#include "RenderableObjectSystem.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/RenderableModel.h>
#include <Engine/RenderableResource.h>

#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/ComponentTypes.h>

void sad::ecs::RenderableObjectSystem::Update(EntityWorld& world)
{
	CreateRenderableModels(world);

	CreateRenderablePrimitives(world);
}

void sad::ecs::RenderableObjectSystem::CreateRenderableModels(EntityWorld& world)
{
	auto view = world.view<ModelResourceComponent, TransformComponent>();
	for (auto [handle, modelResourceComponent, transformComponent] : view.each())
	{
		if (!modelResourceComponent.m_IsResourceDirty)
			continue;

		ecs::Entity entity = ecs::Entity(handle);

		// Import the model and its geometry, attach it to the entity and mark it as a renderable
		std::string modelFilePath = modelResourceComponent.m_Model->GetResourceAbsolutePath();
		RenderableModel model = RenderableModel(modelFilePath);
		entity.AddComponent<RenderableModelComponent>(model);

		// Retrieve the bound from the model and apply it to the entity
		// Something has to happen in the Bound for this to be applied correctly
		aiAABB aabb = model.GetBound();
		glm::vec3 min = glm::vec3(aabb.mMin[0], aabb.mMin[1], aabb.mMin[2]);
		glm::vec3 max = glm::vec3(aabb.mMax[0], aabb.mMax[1], aabb.mMax[2]);
		BoundComponent bound = entity.GetComponent<BoundComponent>();
		if (bound.m_Bound->GetSizeDiff() == glm::vec3(0, 0, 0))
		{
			bound.m_Bound->SetMinMax(min, max);
			bound.m_Bound->SetSizeRatio(transformComponent.m_Transform->GetScale());
		}

		// Mark the RenderablePrimitive as clean
		modelResourceComponent.m_IsResourceDirty = false;
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

		// Mark the RenderablePrimitive as clean
		primitiveResourceComponent.m_IsResourceDirty = false;
	}
}

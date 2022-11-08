#include "sadpch.h"

#include "Entity.h"

sad::ecs::Entity::Entity()
{ 
	Transform = core::CreatePointer<sad::Transform>();
	Bound = core::CreatePointer<sad::Bound>(std::move(*Transform.get()));
	m_EntityHandle = sad::ecs::Registry::GenerateEntityHandle();
}
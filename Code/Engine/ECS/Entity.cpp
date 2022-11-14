#include "sadpch.h"

#include "Entity.h"

sad::ecs::Entity::Entity()
	: m_EntityHandle(sad::ecs::Registry::GenerateEntityHandle())
{ 
	//Transform = core::CreatePointer<sad::Transform>();
	//Bound = core::CreatePointer<sad::Bound>(std::move(*Transform.get()));
	//m_EntityHandle = sad::ecs::Registry::GenerateEntityHandle();
}

sad::ecs::Entity::Entity(entt::entity entityHandle)
	: m_EntityHandle(entityHandle)
{
}

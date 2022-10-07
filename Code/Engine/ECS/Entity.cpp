#include "sadpch.h"

#include "Entity.h"

sad::ecs::Entity::Entity()
{ 
	m_EntityHandle = sad::ecs::Registry::GenerateEntityHandle();
}
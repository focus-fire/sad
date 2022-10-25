#include "sadpch.h"

#include "Entity.h"

sad::ecs::Entity::Entity()
	: Transform(sad::Transform())
{ 
	m_EntityHandle = sad::ecs::Registry::GenerateEntityHandle();
}
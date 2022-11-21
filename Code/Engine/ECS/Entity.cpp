#include "sadpch.h"

#include "Entity.h"

sad::ecs::Entity::Entity(entt::null_t null)
	: m_EntityHandle(null)
{ }

sad::ecs::Entity::Entity(entt::entity entityHandle)
	: m_EntityHandle(entityHandle)
{ }

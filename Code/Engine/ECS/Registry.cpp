#include "sadpch.h"

#include "Registry.h"

sad::ecs::Registry& sad::ecs::Registry::GetInstance()
{
	static Registry instance;
	return instance;
}

sad::ecs::EntityWorld& sad::ecs::Registry::GetEntityWorld()
{
	return GetInstance().World;
}

entt::entity sad::ecs::Registry::GenerateEntityHandle()
{
	return GetInstance().World.create();
}
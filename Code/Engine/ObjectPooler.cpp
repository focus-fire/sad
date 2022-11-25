#pragma once

#include "sadpch.h"
#include "ObjectPooler.h"
#include "ECS/Entity.h"

sad::ObjectPooler& sad::ObjectPooler::GetInstance()
{
	static sad::ObjectPooler instance;
	return instance;
}

sad::ecs::Entity* sad::ObjectPooler::GetFromPool()
{
	core::Log(ELogType::Info, "Getting from pool");
	return &m_Pool[GetAvailableIndex()].entity;
}

void sad::ObjectPooler::ReturnToPool(ecs::Entity* entity)
{
	core::Log(ELogType::Info, "Returning to pool");
}

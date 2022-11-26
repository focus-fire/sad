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
	core::Log(ELogType::Info, "Getting from pool at index {} : {}", GetAvailableIndex(), 
		static_cast<void*>(&m_Pool[GetAvailableIndex()].entity));
	m_Pool[GetAvailableIndex()].isAvailable = false;
	return &m_Pool[GetAvailableIndex()].entity;
}

void sad::ObjectPooler::ReturnToPool(ecs::Entity entity)
{
	for (int i = 0; i < m_PoolSize; i++)
	{
		core::Log(ELogType::Info, "{}:\tPool: {} / Entity: {}", i, static_cast<void*>(&m_Pool[i].entity), static_cast<void*>(&entity));
		if (&m_Pool[i].entity == &entity && !m_Pool[i].isAvailable)
		{
			m_Pool[i].isAvailable = true;
			core::Log(ELogType::Info, "Successfully returned to pool at index {}", i);
			return;
		}	
	}
	core::Log(ELogType::Info, "Failed to return to pool");
}

void sad::ObjectPooler::ExpandEntityPool()
{
	for (int i = 0; i < m_PoolSize; i++)
	{
		m_Pool.push_back(PoolObject());
	}
	m_PoolSize *= 2;
}

int sad::ObjectPooler::GetAvailableIndex()
{
	int i;
	for (i = 0; i < m_PoolSize; i++)
	{
		if (m_Pool[i].isAvailable == true)
			return i;
	}
	ExpandEntityPool();
	return i + 1;
}


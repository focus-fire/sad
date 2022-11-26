#pragma once

#include <vector>
#include <map>

#include "ECS/Entity.h"

namespace sad
{
	struct PoolObject
	{
		ecs::Entity entity;
		bool isAvailable;

		PoolObject()
		{
			entity = ecs::Entity(ecs::Registry::GenerateEntityHandle());
			isAvailable = true;
		}
	};

	class ObjectPooler
	{
	public:
		static sad::ObjectPooler& GetInstance();
		ObjectPooler(const ObjectPooler&) = delete;

		/**
		 * @brief Get a pointer to the next available entity in the pool, mark as active
		 * @return 
		*/
		ecs::Entity* GetFromPool();

		/**
		 * @brief Return a pointer to the entity pool, mark as inactive
		 * @param entity 
		*/
		void ReturnToPool(ecs::Entity entity);

	private:
		ObjectPooler()
		{
			for (int i = 0; i < m_PoolSize; i++)
			{
				m_Pool.push_back(PoolObject());
			}
		};
		static sad::ObjectPooler s_ObjectPooler;

		std::vector<PoolObject> m_Pool;
		int m_PoolSize = 5;

		/**
		 * @brief Doubles the entity pool size
		*/
		void ExpandEntityPool();

		/**
		 * @brief Return the first available index, return -1 if not found
		 * @return 
		*/
		int GetAvailableIndex();
	};
}
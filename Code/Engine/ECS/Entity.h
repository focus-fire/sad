#pragma once

#include <entt/entt.hpp>

#include "Registry.h"

namespace sad::ecs
{
	class Entity 
	{
	public:
		Entity();
		Entity(const Entity& entity) = delete;

		entt::entity GetHandle() { return m_EntityHandle; }

		template<typename T>
		T& AddComponent(T component)
		{
			return Registry::GetEntityWorld()->emplace<T>(m_EntityHandle, component);
		}

		template<typename T>
		T& RemoveComponent()
		{
			return Registry::GetEntityWorld()->remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			return Registry::GetEntityWorld()->get<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle;
	};
}

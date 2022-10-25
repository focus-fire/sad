#pragma once

#include <entt/entt.hpp>

#include <Engine/Transform.h>

#include "Registry.h"

namespace sad::ecs
{
	class Entity 
	{
	public:
		Entity();
		Entity(const Entity& entity) = delete;

		const entt::entity GetHandle() const { return m_EntityHandle; }

		template<typename T>
		T& AddComponent(T component)
		{
			return Registry::GetEntityWorld().emplace<T>(m_EntityHandle, component);
		}

		template<typename T>
		void AddEmptyComponent(T component)
		{
			return Registry::GetEntityWorld().emplace<T>(m_EntityHandle, component);
		};

		template<typename T>
		T& RemoveComponent()
		{
			return Registry::GetEntityWorld().remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			return Registry::GetEntityWorld().get<T>(m_EntityHandle);
		}

		bool operator==(const Entity& other) const
		{ 
			return m_EntityHandle == other.m_EntityHandle;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	public:
		Transform Transform;

	private:
		entt::entity m_EntityHandle;
	};
}

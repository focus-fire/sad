#pragma once

#include <entt/entt.hpp>

#include <Core/Memory.h>

#include <Engine/Bound.h>
#include <Engine/Transform.h>

#include "Registry.h"

namespace sad::ecs
{
	/**
	 * @brief Wrapper class for an entt::entity, contains utility methods 
	 *		  to update the entities components in the EntityWorld
	*/
	class Entity 
	{
	public:
		Entity();
		Entity(const Entity& entity) = delete;

		/**
		 * @brief Retrieves the entt::entity handle created for the entity
		*/
		const entt::entity GetHandle() const { return m_EntityHandle; }

		/**
		 * @brief Adds a component to the current entity 
		 * @tparam T Generic struct component to add
		 * @param component Component to add to the entity
		 * @return Reference to the added component
		*/
		template<typename T>
		T& AddComponent(T component)
		{
			return Registry::GetEntityWorld().emplace<T>(m_EntityHandle, component);
		}

		/**
		 * @brief Adds an empty 'tag' component to the current entity
		 * @tparam T Generic struct for an empty component to add
		 * @param component Component to add to the entity
		*/
		template<typename T>
		void AddEmptyComponent(T component)
		{
			Registry::GetEntityWorld().emplace<T>(m_EntityHandle, component);
		};

		/**
		 * @brief Removes a component from the current entity
		 * @tparam T Generic struct component to remove
		 * @return Reference to the removed component
		*/
		template<typename T>
		void RemoveComponent()
		{
			Registry::GetEntityWorld().remove<T>(m_EntityHandle);
		}

		/**
		 * @brief Retrieves a specified component from the entity 
		 * @tparam T Generic struct component to retrieve 
		 * @return Reference to the retrieved component
		*/
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
		core::Pointer<Transform> Transform;
		core::Pointer<Bound> Bound;

	private:
		entt::entity m_EntityHandle;
	};
}

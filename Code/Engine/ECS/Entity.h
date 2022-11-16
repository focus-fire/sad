#pragma once

#include <entt/entt.hpp>

#include <Core/Memory.h>
#include <Core/Guid.h>

#include <Engine/Bound.h>
#include <Engine/Transform.h>

#include "Registry.h"
#include "Components/GuidComponent.h"

namespace sad::ecs
{
	/**
	 * @brief Wrapper class for an entt::entity, contains utility methods 
	 *		  to update the entities components in the EntityWorld
	*/
	class Entity 
	{
	public:
		Entity() = default;
		Entity(entt::entity entityHandle);
		Entity(entt::null_t null);
		Entity(const Entity& entity) = default;

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
		 * @brief Potentially overwrites a component on the entity with the component passed
		 * @tparam T Generic struct component to add
		 * @param component Component to add or replace on the entity
		 * @return Reference to the added component
		*/
		template<typename T>
		T& OverwriteComponent(T component)
		{
			return Registry::GetEntityWorld().emplace_or_replace<T>(m_EntityHandle, component);
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
			SAD_ASSERT(HasComponent<T>(), "Attempting to remove a component that does not exist on the entity");
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

		/**
		 * @brief Checks if a particular component exists on an entity
		 * @tparam T Generic struct component to evaluate the existence of
		 * @return True if the component exists on the entity, false if it does not 
		*/
		template<typename T>
		bool HasComponent()
		{
			return Registry::GetEntityWorld().all_of<T>(m_EntityHandle);
		}

		/**
		 * @brief Retrieves native GUID attached to the entity
		 * @return Native GUID associated with the entity's GuidComponent
		*/
		core::Guid GetGuid() { return this->GetComponent<ecs::GuidComponent>().m_GUID; }

		operator bool() const 
		{ 
			return m_EntityHandle != entt::null; 
		}

		operator entt::entity() const 
		{ 
			return m_EntityHandle; 
		}

		bool operator==(const Entity& other) const
		{ 
			return m_EntityHandle == other.m_EntityHandle;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		static ecs::Entity Null() { return ecs::Entity(entt::null); }

	private:
		entt::entity m_EntityHandle;
	};
}

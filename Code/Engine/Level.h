#pragma once

#include <unordered_map>

#include <entt/entt.hpp>

#include <Core/Guid.h>

#include "ECS/Entity.h"
#include "ECS/Registry.h"

namespace sad
{
	class Level
	{
	public:
		Level() = default;

		void Update(ecs::EntityWorld& world);

		/**
		 * @brief Cycles through all entities and performs an import on the entity
		 * @note This is currently only used after level import to populate entities with GUIDComponents.
		*/
		void PopulateLevelGuids();

		/**
		 * @brief Imports an enitity handle into the level cache with a GUIDComponent
		 * @param handle The entt handle corresponding with the imported entity
		 * @param guid Native GUID for the entity, default parameter value will generate one automatically
		 * @return Reference to the imported entity
		*/
		ecs::Entity ImportEntityFromHandle(entt::entity handle, core::Guid guid = core::Guid::CreateGuid());

		/**
		 * @brief Creates an entity in the level from scratch with a new GUID, an empty transform, and a fresh bound
		 * @return Reference to the instantiated entity
		*/
		ecs::Entity InstantiateEntity();

		/**
		 * @brief Creates an entity in the level from a provided entt handle and/or a guid
		 * @param handle Valid entt handle
		 * @param guid Native GUID to attach to the entity in the level
		 * @return Reference to the instantiated entity
		*/
		ecs::Entity InstantiateEntityFromHandle(entt::entity handle, core::Guid guid = core::Guid::CreateGuid());

		/**
		 * @brief Destroys an entity from the registry and from the level cache
		 * @param entity Handle associated with the entity requested for removal
		*/
		void DestroyEntity(ecs::Entity entity);

	private:
		std::unordered_map<core::Guid, entt::entity> m_EntityMap;
	};
}

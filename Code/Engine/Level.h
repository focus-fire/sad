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

		void Start();
		void Update(ecs::EntityWorld& world);

		/**
		 * @brief Cycles through all entities and performs an import on the entity
		 * @note This is currently only used after level import to populate entities with GUIDComponents.
		*/
		void PopulateLevelGuids();

		/**
		 * @brief Retrieves an entity in the level by GUID
		*/
		ecs::Entity LookupEntityByGuid(const core::Guid& guid);

		/**
		 * @brief Retrieves an entity in the level by name 
		*/
		ecs::Entity LookupEntityByName(const std::string& name);

		/**
		 * @brief Creates an entity in the level from scratch with a new GUID, an empty transform, and a fresh bound
		 * @param name Name used to identify the entity in the engine
		 * @return Reference to the instantiated entity
		*/
		ecs::Entity InstantiateEntity(const std::string& name);

		/**
		 * @brief Creates an entity in the level from a provided entt handle and/or a guid
		 * @param handle Valid entt handle
		 * @param guid Native GUID to attach to the entity in the level
		 * @return Reference to the instantiated entity
		*/
		ecs::Entity InstantiateEntityFromHandle(entt::entity handle, const std::string& name, core::Guid guid = core::Guid::CreateGuid());

		/**
		 * @brief Destroys an entity from the registry and from the level cache
		 * @param entity Handle associated with the entity requested for removal
		 * @return True if the entity was successfully destroyed, false if it was not
		*/
		bool DestroyEntity(ecs::Entity entity);

		/**
		 * @brief Destroys an entity from the registry and from the level cache with a particular name
		 * @param name Name associated with the entity requested for removal 
		 * @return True if the entity was successfully destroyed, false if it was not
		*/
		bool DestroyEntityByName(const std::string& name);

		/**
		 * @brief Imports an enitity handle into the level cache with a GUIDComponent
		 * @param handle The entt handle corresponding with the imported entity
		 * @param guid Native GUID for the entity, default parameter value will generate one automatically
		 * @return Reference to the imported entity
		*/
		ecs::Entity ImportEntityFromHandle(entt::entity handle, core::Guid guid = core::Guid::CreateGuid());

	public:
		/**
		 * @brief Name of the level file 
		*/
		std::string LevelName;

	private:
		std::unordered_map<core::Guid, entt::entity> m_EntityMap;
	};
}

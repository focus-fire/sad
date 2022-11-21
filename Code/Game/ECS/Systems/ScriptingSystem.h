#pragma once

#include <Engine/ECS/Registry.h>

namespace pog::ecs
{
	/**
	 * @brief Responsible for awaking and updating all entities with a ScriptComponent
	*/
	class ScriptingSystem
	{	
	public:
		/**
		 * @brief Calls Awake() via the ScriptingEngine on all SadBehaviours attached to entities 
		*/
		static void Awake(sad::ecs::EntityWorld& world);

		/**
		 * @brief Calls Update() via the ScriptingEngine on all SadBehaviours attached to entities 
		*/
		static void Update(sad::ecs::EntityWorld& world);
	};
}

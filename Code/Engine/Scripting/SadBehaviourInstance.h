#pragma once

#include <mono/metadata/metadata.h>

#include <Core/Memory.h>

#include <Engine/ECS/Entity.h>

#include "ScriptClass.h"

namespace sad::cs
{
	/**
	 * @brief Representation of a SadBehaviour script instantiated in the Mono runtime
	*/
	class SadBehaviourInstance
	{
	public:
		SadBehaviourInstance() = default;
		SadBehaviourInstance(core::Pointer<ScriptClass> sadBehaviourScript, ecs::Entity entity);

		/**
		 * @brief Calls (or 'invokes') the Awake() method if it exists on a SadBehaviour
		*/
		void CallAwake();

		/**
		 * @brief Calls (or 'invokes') the Update() method if it exists on a SadBehaviour
		*/
		void CallUpdate();

		/**
		 * @brief Calls (or 'invokes') the DrawGizmos() method if it exists on a SadBehaviour
		*/
		void CallDrawGizmos();

		/**
		 * @brief Retrieves the script definiition for this SadBehaviour instance
		*/
		ScriptClass* GetScriptDefinition() { return m_SadBehaviourScript.get(); }

		/**
		 * @brief Retrieves the managed instance (object) for this SadBehaviour 
		*/
		MonoObject* GetManagedInstance() { return m_Instance; }

	private:
		core::Pointer<ScriptClass> m_SadBehaviourScript;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_Awake = nullptr;
		MonoMethod* m_Update = nullptr;
		MonoMethod* m_DrawGizmos = nullptr;
	};
}

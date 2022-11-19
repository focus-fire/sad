#pragma once

#include <mono/metadata/metadata.h>

#include <Core/Memory.h>

#include "ScriptingEngine.h"
#include "ScriptClass.h"

namespace sad::cs
{
	class SadBehaviourInstance
	{
	public:
		SadBehaviourInstance() = default;
		SadBehaviourInstance(core::Pointer<ScriptClass> sadBehaviourScript, ecs::Entity entity)
			: m_SadBehaviourScript(sadBehaviourScript)
		{
			// Initialize the instance of the SadBehaviour and retrieve it's lifecycle methods
			m_Instance = sadBehaviourScript->Instantiate();

			// Call the SadBehaviour GUID constructor
			m_Constructor = sad::cs::ScriptingEngine::s_ScriptingData->SadBehaviourClass.GetMethod(".ctor", 1);

			// Initialize lifecycle methods on script
			m_Awake = sadBehaviourScript->GetMethod("Awake", 0);
			m_Update = sadBehaviourScript->GetMethod("Update", 0);


			// Initialize all SadBehaviours with their entity's guid
			{
				core::Guid guid = entity.GetGuid();

				// May be of type GUID (Win32) or uuid_t (Unix)
				// Note: uuid_t may not marshall properly to a System.Guid
				core::NativeGuid nGuid = guid.GetNativeGuid();
				void* param = &nGuid;
				m_SadBehaviourScript->CallMethod(m_Constructor, m_Instance, &param);
			}
		}

		void CallAwake() { m_SadBehaviourScript->CallMethod(m_Awake, m_Instance); }
		void CallUpdate() { m_SadBehaviourScript->CallMethod(m_Update, m_Instance); }

	private:
		core::Pointer<ScriptClass> m_SadBehaviourScript;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_Awake = nullptr;
		MonoMethod* m_Update = nullptr;
	};
}

#pragma once

#include <mono/metadata/metadata.h>

#include <Core/Memory.h>

#include "ScriptClass.h"

namespace sad::cs
{
	class SadBehaviourInstance
	{
	public:
		SadBehaviourInstance() = default;
		SadBehaviourInstance(core::Pointer<ScriptClass> sadBehaviourScript)
			: m_SadBehaviourScript(sadBehaviourScript)
		{
			m_Instance = sadBehaviourScript->Instantiate();
			m_Awake = sadBehaviourScript->GetMethod("Awake", 0);
			m_Update = sadBehaviourScript->GetMethod("Update", 0);
		}

		void CallAwake() { m_SadBehaviourScript->CallMethod(m_Awake, m_Instance); }
		void CallUpdate() { m_SadBehaviourScript->CallMethod(m_Update, m_Instance); }

	private:
		core::Pointer<ScriptClass> m_SadBehaviourScript;

		MonoObject* m_Instance;
		MonoMethod* m_Awake;
		MonoMethod* m_Update;
	};
}

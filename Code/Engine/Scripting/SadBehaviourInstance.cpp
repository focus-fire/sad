#include "sadpch.h"

#include "SadBehaviourInstance.h"

#include "ScriptingEngine.h"

sad::cs::SadBehaviourInstance::SadBehaviourInstance(core::Pointer<ScriptClass> sadBehaviourScript, ecs::Entity entity)
	: m_SadBehaviourScript(sadBehaviourScript)
{
	// Initialize the instance of the SadBehaviour and retrieve it's lifecycle methods
	m_Instance = sadBehaviourScript->Instantiate();

	// Call the SadBehaviour GUID constructor
	m_Constructor = sad::cs::ScriptingEngine::s_ScriptingData->SadBehaviourClass.GetMethod(".ctor", 1);

	// Initialize lifecycle methods on script
	m_Awake = sadBehaviourScript->GetMethod("Awake", 0);
	m_Update = sadBehaviourScript->GetMethod("Update", 0);
	m_DrawGizmos = sadBehaviourScript->GetMethod("DrawGizmos", 0);

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

void sad::cs::SadBehaviourInstance::CallAwake() 
{ 
	if (!m_Awake)
		return;

	m_SadBehaviourScript->CallMethod(m_Awake, m_Instance); 
}

void sad::cs::SadBehaviourInstance::CallUpdate() 
{ 
	if (!m_Update)
		return;

	m_SadBehaviourScript->CallMethod(m_Update, m_Instance); 
}

void sad::cs::SadBehaviourInstance::CallDrawGizmos()
{
	if (!m_DrawGizmos)
		return;

	m_SadBehaviourScript->CallMethod(m_DrawGizmos, m_Instance);
}

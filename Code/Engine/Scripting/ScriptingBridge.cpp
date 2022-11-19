#include "sadpch.h"

#include "ScriptingBridge.h"

#include <glm/glm.hpp>
#include <mono/metadata/object.h>

#include <Engine/ECS/Components/ComponentTypes.h>

#include "ScriptingEngine.h"

/// Refer to SadCSFramework.Internal.cs for a reference on currently implemented internal methods
/// In order to add a new method to the scripting api, use this macro with the className (in the 'Sad.Internal' namespace) and the method name
/// Note: Parameters can be added to the 'mono_add_internal_call' signature in order to define an overload
/// ie: mono_add_internal_call("Sad.Internal.Log::Debug(string)") vs mono_add_internal_call("Sad.Internal.Log::Debug(Vector3)")
#define SAD_CSF_ADD_INTERNAL(className, method) mono_add_internal_call("Sad.Internal." className "::" #method, (const void*) method)

namespace sad::cs
{
	///////////
	/// Log ///
	///////////

	static void Info(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Info, "{}", cString);

		mono_free(cString);
	}

	static void Debug(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Debug, "{}", cString);

		mono_free(cString);
	}

	static void Warn(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Warn, "{}", cString);

		mono_free(cString);
	}

	static void Error(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Error, "{}", cString);

		mono_free(cString);
	}

	///////
	///////
	///////

	//////////////
	/// Entity ///
	//////////////

	static void GetPosition(core::NativeGuid guid, glm::vec3* outPosition)
	{
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		ecs::Entity entity = level->GetEntityByGuid(guid);

		*outPosition = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetPosition();
	}

	static void SetPosition(core::NativeGuid guid, glm::vec3* position)
	{
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		ecs::Entity entity = level->GetEntityByGuid(guid);

		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetPosition(*position);
	} 
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	SAD_CSF_ADD_INTERNAL("Log", Debug);
	SAD_CSF_ADD_INTERNAL("Log", Warn);
	SAD_CSF_ADD_INTERNAL("Log", Error);

	SAD_CSF_ADD_INTERNAL("Transform", GetPosition);
	SAD_CSF_ADD_INTERNAL("Transform", SetPosition);
}

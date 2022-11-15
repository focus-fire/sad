#include "sadpch.h"

#include "ScriptingBridge.h"

#include <mono/metadata/object.h>

#define SAD_CSF_ADD_CORE_INTERNAL(name) mono_add_internal_call("Sad.Core::" #name, name)

namespace sad::cs
{
	static void Log(MonoString* string)
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

	static void Play(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);
		
		// Ad
	}
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	//
	SAD_CSF_ADD_CORE_INTERNAL(Log);
	SAD_CSF_ADD_CORE_INTERNAL(Warn);
}

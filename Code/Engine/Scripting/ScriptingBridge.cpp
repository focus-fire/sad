#include "sadpch.h"

#include "ScriptingBridge.h"

#include <mono/metadata/object.h>

#define SAD_CSF_ADD_CORE_INTERNAL(name) mono_add_internal_call("Sad.Core::" #name, (const void*) name)

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
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	// Core Internal Calls
	SAD_CSF_ADD_CORE_INTERNAL(Log);
	SAD_CSF_ADD_CORE_INTERNAL(Warn);
}

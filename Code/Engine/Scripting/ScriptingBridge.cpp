#include "sadpch.h"

#include "ScriptingBridge.h"

#include <mono/metadata/object.h>

#define SAD_CSF_ADD_INTERNAL_CALL(name) mono_add_internal_call("Sad.Core::" #name, name)

namespace sad::cs
{
	static void Log(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Debug, "{}", cString);

		mono_free(cString);
	}
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	SAD_CSF_ADD_INTERNAL_CALL(Log);
}

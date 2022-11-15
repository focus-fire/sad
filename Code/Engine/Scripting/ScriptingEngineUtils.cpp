#include "sadpch.h"

#include "ScriptingEngineUtils.h"

#include <mono/jit/jit.h>

MonoAssembly* sad::cs::ScriptingEngineUtils::LoadCSharpAssembly(const std::string& assemblyPath)
{
	uint32_t size = 0;
	char* bytes = core::FileUtils::ReadBytes(assemblyPath, &size);
	
	// This image can only be used for loading the assmebly since it doesn't have a direct reference
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(bytes, size, 1, &status, 0);
	SAD_ASSERT(status == MONO_IMAGE_OK, "Mono failed to load an image from the corresponding assembly");

	if (status != MONO_IMAGE_OK)
	{
		const char* error = mono_image_strerror(status);
		core::Log(ELogType::Error, "[ScriptingUtils] Mono failed to load an image from an assembly at {} with error {}", assemblyPath, error);
		return nullptr;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
	mono_image_close(image);

	delete[] bytes;

	return assembly;
}

void sad::cs::ScriptingEngineUtils::PrintAssemblyTypes(MonoAssembly* assembly)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t numberOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numberOfTypes; ++i)
	{
		uint32_t columns[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, columns, MONO_TYPEDEF_SIZE);

		const char* nameSpace = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAMESPACE]);
		const char* name = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAME]);

		core::Log(ELogType::Info, "[ScriptingUtils] {}.{}", nameSpace, name);
	}
}

sad::cs::ScriptableType sad::cs::ScriptingEngineUtils::MonoTypeToScriptableType(MonoType* type)
{
	std::string typeName = mono_type_get_name(type);
	
	auto it = s_MonoScriptableTypeLookup.find(typeName);
	if (it == s_MonoScriptableTypeLookup.end())
	{
		core::Log(ELogType::Error, "[ScriptingUtils] Attempted to lookup undefined type {}", typeName);
		return ScriptableType::None;
	}

	return it->second;
}

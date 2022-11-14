#include "sadpch.h"

#include "ScriptingEngineUtils.h"

#include <mono/jit/jit.h>

MonoAssembly* sad::cs::ScriptingEngineUtils::LoadCSharpAssembly(const std::string& assemblyPath)
{
	uint32_t size = 0;
	char* bytes = ReadBytes(assemblyPath, &size);
	
	// This image can only be used for loading the assmebly since it doesn't have a direct reference
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(bytes, size, 1, &status, 0);
	SAD_ASSERT(status == MONO_IMAGE_OK, "Mono failed to load an image from the corresponding assembly");

	if (status != MONO_IMAGE_OK)
	{
		const char* error = mono_image_strerror(status);
		core::Log(ELogType::Error, "[ScriptingEngine] Mono failed to load an image from an assembly at {} with error {}", assemblyPath, error);
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

		core::Log(ELogType::Debug, "[Scripting] {}.{}", nameSpace, name);
	}
}

char* sad::cs::ScriptingEngineUtils::ReadBytes(const std::string& filePath, uint32_t* outputSize)
{
	// End the stream immediately after opening it
	std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ios::ate);
	SAD_ASSERT(fileStream, "Failed to open stream for Mono assembly");

	// Get byte size by subtracting end and beginning of stream
	std::streampos end = fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);
	uint32_t size = end - fileStream.tellg();
	SAD_ASSERT(size != 0, "Stream for Mono assembly is empty (loaded assembly contained 0 bytes)");

	// Read stream into a buffer of bytes
	char* buffer = new char[size];
	fileStream.read((char*) buffer, size);
	fileStream.close();

	// Assign the number of bytes for the returned buffer
	*outputSize = size;

	return buffer;
}

#include "sadpch.h"

#include "ScriptingEngine.h"

#include <mono/metadata/assembly.h>

void sad::cs::ScriptingEngine::Start()
{
	m_ScriptingConfig = new ScriptingEngineConfig();
	
	StartMono();
}

void sad::cs::ScriptingEngine::Teardown()
{
	delete m_ScriptingConfig;

	TeardownMono();
}

void sad::cs::ScriptingEngine::StartMono()
{
	// Sets path for cs assemblies relative to the current working directory
	// This could be set as a config variable and modified by engine users (ie: if a local set of assemblies is installed)
	// For the scope of this project though it probably makes sense to leave it in the project
	mono_set_assemblies_path("SadCSFramework/.net/lib");

	// Create root domain for JIT runtime
	MonoDomain* csRootDomain = mono_jit_init("sadJITRuntime");
	SAD_ASSERT(csRootDomain, "Failed to initialize Mono JIT runtime");
	m_ScriptingConfig->RootDomain = csRootDomain;

	// Create app domain for Mono
	char appDomainName[] = "sadScriptRuntime";
	m_ScriptingConfig->AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(m_ScriptingConfig->AppDomain, true);

	// Testing, load and print assembly types 
	std::string assemblyPath = core::FileUtils::GetPathInsideCodeDirectory("SadCSFramework/Build/Scripts/SadCSFramework.dll");
	m_ScriptingConfig->SadCSFrameworkAssembly = LoadCSharpAssembly(assemblyPath);
	PrintAssemblyTypes(m_ScriptingConfig->SadCSFrameworkAssembly);

	// Testing, retrieve class from CS assembly and allocate it in memory
	MonoObject* testObject = InstantiateClass("Sad", "HelloWorld");
	CallTestMethod(testObject);
	CallIncrementTestMethod(testObject, 1);
	CallTestMethod(testObject);
}

void sad::cs::ScriptingEngine::TeardownMono()
{ }

char* sad::cs::ScriptingEngine::ReadBytes(const std::string& filePath, uint32_t* outputSize)
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

MonoAssembly* sad::cs::ScriptingEngine::LoadCSharpAssembly(const std::string& assemblyPath)
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

void sad::cs::ScriptingEngine::PrintAssemblyTypes(MonoAssembly* assembly)
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

MonoClass* sad::cs::ScriptingEngine::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	MonoClass* aClass = mono_class_from_name(image, namespaceName, className);
	SAD_ASSERT(aClass, "Failed to retrieve class name from CSharp assembly");

	return aClass;
}

MonoObject* sad::cs::ScriptingEngine::InstantiateClass(const char* namespaceName, const char* className)
{
	MonoClass* testClass = GetClassInAssembly(m_ScriptingConfig->SadCSFrameworkAssembly, namespaceName, className);
	MonoObject* classInstance = mono_object_new(m_ScriptingConfig->AppDomain, testClass);
	SAD_ASSERT(classInstance, "Mono failed to allocate a new object-type for a class");

	// Actually calls the default constructor for HelloWorld
	// Preferable to make the assumption that all SadCSFramework classes will be default constructible
	// If a constructor isn't explicitly constructed at all the C# compiler adds one 
	// Otherwise ensure one exists in the event multiple constructors are used   
	mono_runtime_object_init(classInstance);

	return classInstance;
}

///////////////
/// Testing ///
///////////////

void sad::cs::ScriptingEngine::CallTestMethod(MonoObject* objectInstance)
{
	// Get class and method references off of passed object...
	MonoClass* classInstance = mono_object_get_class(objectInstance);

	// Retrieve method with particular signature
	// Third integer indicates the number of parameters the method has
	// Despite this, Mono retrieves this method ambiguously 
	// Thus, if a class has two methods with the same number of parameters, it has to be handled differently
	MonoMethod* method = mono_class_get_method_from_name(classInstance, "PrintIntegerVariable", 0);
	SAD_ASSERT(method, "Mono failed to retrieve symbol in CS assembly for requested method");

	// Call the C# method on the object
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, objectInstance, nullptr, &exception);
	SAD_ASSERT(!exception, "Unhandled exception occurred while executing method from CS assembly");
}

void sad::cs::ScriptingEngine::CallIncrementTestMethod(MonoObject* objectInstance, int value)
{
	MonoClass* classInstance = mono_object_get_class(objectInstance);
	MonoMethod* method = mono_class_get_method_from_name(classInstance, "Increment", 1);
	SAD_ASSERT(method, "Mono failed to retrieve symbol in CS assembly for requested method");
	
	// Pass params by marshalling data to CSharp
	// Can also be... void* param[] = { &value, &value2, ... , &valueN };
	void* param = &value;

	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, objectInstance, &param, &exception);
	SAD_ASSERT(!exception, "Unhandled exception occurred while executing method from CS assembly");
}

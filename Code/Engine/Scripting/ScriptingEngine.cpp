#include "sadpch.h"

#include "ScriptingEngine.h"

#include <mono/metadata/assembly.h>

#include "ScriptingBridge.h"
#include "ScriptingEngineUtils.h"

sad::cs::ScriptingEngine::ScriptingEngineConfig* sad::cs::ScriptingEngine::s_ScriptingConfig = nullptr;

void sad::cs::ScriptingEngine::Start()
{
	s_ScriptingConfig = new ScriptingEngineConfig();
	
	StartMono();

	// Register functions in the engine API
	ScriptingBridge::SetupEngineAPIFunctions();

	// Load the SadCSFramework assembly
	std::string sadCSFrameworkAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Resources/SadCSFramework/SadCSFramework.dll");
	LoadSadCSFrameworkAssembly(sadCSFrameworkAssemblyPath);

	// TODO: Create an assembly for the scripting project and mount it
	// std::string projectAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Assets/Scripts/Assembly/Project.dll");
	// LoadProjectAssembly(projectAssemblyPath);

	// Testing, print the types that exist in the loaded assembly
	ScriptingEngineUtils::PrintAssemblyTypes(s_ScriptingConfig->SadCSFrameworkAssembly);

	// Testing, retrieve class from CS assembly and allocate it in memory
	// MonoObject* testObject = InstantiateClass("", "HelloWorld");
}

void sad::cs::ScriptingEngine::Teardown()
{
	TeardownMono();

	delete s_ScriptingConfig;
}

void sad::cs::ScriptingEngine::RuntimeStart(sad::Level* level)
{
	s_ScriptingConfig->CurrentLevelInstance = level;
}

void sad::cs::ScriptingEngine::RuntimeStop()
{
	s_ScriptingConfig->CurrentLevelInstance = nullptr;
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
	s_ScriptingConfig->RootDomain = csRootDomain;
}

void sad::cs::ScriptingEngine::TeardownMono()
{
	mono_domain_set(mono_get_root_domain(), false);

	mono_domain_unload(s_ScriptingConfig->AppDomain);
	s_ScriptingConfig->AppDomain = nullptr;

	mono_jit_cleanup(s_ScriptingConfig->RootDomain);
	s_ScriptingConfig->RootDomain = nullptr;
}

//////////////////////////////////
/// Assembly Loading/Unloading ///
//////////////////////////////////

void sad::cs::ScriptingEngine::LoadSadCSFrameworkAssembly(const std::string& filePath)
{
	// Create domain for the SadCSFramework runtime
	char appDomainName[] = "SadCSFrameworkScriptRuntime";
	s_ScriptingConfig->AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(s_ScriptingConfig->AppDomain, true);

	// Save a reference to the assembly
	s_ScriptingConfig->SadCSFrameworkAssemblyFilePath = filePath;
	s_ScriptingConfig->SadCSFrameworkAssembly = ScriptingEngineUtils::LoadCSharpAssembly(filePath);
	SAD_ASSERT(s_ScriptingConfig->SadCSFrameworkAssembly, "Failed to load assembly for SadCSFramework");

	// Save a reference to image for the assembly
	s_ScriptingConfig->SadCSFrameworkImage = mono_assembly_get_image(s_ScriptingConfig->SadCSFrameworkAssembly);
	SAD_ASSERT(s_ScriptingConfig->SadCSFrameworkImage, "Failed to mount image for the SadCSFramework assembly");
}

void sad::cs::ScriptingEngine::LoadProjectAssembly(const std::string& filePath)
{
	s_ScriptingConfig->ProjectAssemblyFilePath = filePath;
	s_ScriptingConfig->ProjectAssembly = ScriptingEngineUtils::LoadCSharpAssembly(filePath);
	SAD_ASSERT(s_ScriptingConfig->ProjectAssembly, "Failed to load project assembly");

	s_ScriptingConfig->ProjectImage = mono_assembly_get_image(s_ScriptingConfig->ProjectAssembly);
	SAD_ASSERT(s_ScriptingConfig->ProjectImage, "Failed to mount image for the requested project assembly");
}

///////////////
/// Classes ///
///////////////

MonoClass* sad::cs::ScriptingEngine::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	MonoClass* aClass = mono_class_from_name(image, namespaceName, className);
	SAD_ASSERT(aClass, "Failed to retrieve class name from CSharp assembly");

	return aClass;
}

MonoObject* sad::cs::ScriptingEngine::InstantiateClass(const char* namespaceName, const char* className)
{
	MonoClass* testClass = GetClassInAssembly(s_ScriptingConfig->SadCSFrameworkAssembly, namespaceName, className);
	MonoObject* classInstance = mono_object_new(s_ScriptingConfig->AppDomain, testClass);
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
	MonoMethod* method = mono_class_get_method_from_name(classInstance, "AddNumbers", 2);
	SAD_ASSERT(method, "Mono failed to retrieve symbol in CS assembly for requested method");
	
	// Pass params by marshalling data to CSharp
	// Can also be... void* param[] = { &value, &value2, ... , &valueN };
	// void* param = &value;
	void* param[] = { &value, &value };

	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, objectInstance, param, &exception);
	SAD_ASSERT(!exception, "Unhandled exception occurred while executing method from CS assembly");
}

void sad::cs::ScriptingEngine::CallStringTestMethod(MonoObject* objectInstance, const char* str)
{
	MonoClass* classInstance = mono_object_get_class(objectInstance);
	MonoMethod* method = mono_class_get_method_from_name(classInstance, "AddStrings", 1);
	SAD_ASSERT(method, "Mono failed to retrieve symbol in CS assembly for requested method");
	
	// When passing strings, 
	MonoString* value = mono_string_new(s_ScriptingConfig->AppDomain, str);
	void* param = value;

	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, objectInstance, &param, &exception);
	SAD_ASSERT(!exception, "Unhandled exception occurred while executing method from CS assembly");
}

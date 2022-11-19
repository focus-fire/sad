#include "sadpch.h"

#include "ScriptingEngine.h"

#include <mono/metadata/assembly.h>

#include <Engine/ECS/Components/ComponentTypes.h>

#include "ScriptingBridge.h"
#include "ScriptingEngineUtils.h"
#include "SadBehaviourInstance.h"

sad::cs::ScriptingEngine::ScriptingEngineData* sad::cs::ScriptingEngine::s_ScriptingData = nullptr;

void sad::cs::ScriptingEngine::Start()
{
	s_ScriptingData = new ScriptingEngineData();
	
	StartMono();

	// Load the SadCSFramework assembly and print it's corresponding type information
	std::string sadCSFrameworkAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Resources/SadCSFramework/SadCSFramework.dll");
	LoadSadCSFrameworkAssembly(sadCSFrameworkAssemblyPath);

	// Register functions in the engine API
	ScriptingBridge::SetupEngineAPIFunctions();

	// Cache classes in assembly
	CacheAssemblySadBehaviours(s_ScriptingData->SadCSFrameworkAssembly);

	// TODO: Create an assembly for the scripting project and mount it
	// std::string projectAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Assets/Scripts/Assembly/Project.dll");
	// LoadProjectAssembly(projectAssemblyPath);

	// Hold onto reference of SadBehaviour class
	s_ScriptingData->SadBehaviourClass = ScriptClass("Sad", "SadBehaviour");

	MonoSanityCheck();
}

void sad::cs::ScriptingEngine::Teardown()
{
	TeardownMono();

	delete s_ScriptingData;
}

void sad::cs::ScriptingEngine::RuntimeStart(sad::Level* level)
{
	s_ScriptingData->CurrentLevelInstance = level;
}

void sad::cs::ScriptingEngine::RuntimeStop()
{
	s_ScriptingData->CurrentLevelInstance = nullptr;

	s_ScriptingData->SadBehaviourScriptLookup.clear();
	s_ScriptingData->SadBehaviourInstanceLookup.clear();
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
	s_ScriptingData->RootDomain = csRootDomain;
}

void sad::cs::ScriptingEngine::TeardownMono()
{
	mono_domain_set(mono_get_root_domain(), false);

	mono_domain_unload(s_ScriptingData->AppDomain);
	s_ScriptingData->AppDomain = nullptr;

	mono_jit_cleanup(s_ScriptingData->RootDomain);
	s_ScriptingData->RootDomain = nullptr;
}

//////////////////////////////////
/// Assembly Loading/Unloading ///
//////////////////////////////////

void sad::cs::ScriptingEngine::LoadSadCSFrameworkAssembly(const std::string& filePath)
{
	// Create domain for the SadCSFramework runtime
	char appDomainName[] = "SadCSFrameworkScriptRuntime";
	s_ScriptingData->AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(s_ScriptingData->AppDomain, true);

	// Save a reference to the assembly
	s_ScriptingData->SadCSFrameworkAssemblyFilePath = filePath;
	s_ScriptingData->SadCSFrameworkAssembly = ScriptingEngineUtils::LoadCSharpAssembly(filePath);
	SAD_ASSERT(s_ScriptingData->SadCSFrameworkAssembly, "Failed to load assembly for SadCSFramework");

	// Save a reference to image for the assembly
	s_ScriptingData->SadCSFrameworkImage = mono_assembly_get_image(s_ScriptingData->SadCSFrameworkAssembly);
	SAD_ASSERT(s_ScriptingData->SadCSFrameworkImage, "Failed to mount image for the SadCSFramework assembly");
}

void sad::cs::ScriptingEngine::LoadProjectAssembly(const std::string& filePath)
{
	s_ScriptingData->ProjectAssemblyFilePath = filePath;
	s_ScriptingData->ProjectAssembly = ScriptingEngineUtils::LoadCSharpAssembly(filePath);
	SAD_ASSERT(s_ScriptingData->ProjectAssembly, "Failed to load project assembly");

	s_ScriptingData->ProjectImage = mono_assembly_get_image(s_ScriptingData->ProjectAssembly);
	SAD_ASSERT(s_ScriptingData->ProjectImage, "Failed to mount image for the requested project assembly");
}

//////////////////
/// Entity Ops ///
//////////////////

void sad::cs::ScriptingEngine::AwakeSadBehaviourInstance(ecs::Entity entity)
{
	const ecs::ScriptComponent& scriptComponent = entity.GetComponent<ecs::ScriptComponent>();
	if (SadBehaviourExists(scriptComponent.m_ClassName))
	{
		// Retrieve the base class from the lookup
		core::Pointer<ScriptClass> scriptClass = s_ScriptingData->SadBehaviourScriptLookup[scriptComponent.m_ClassName];

		// Instantiate the script using the class definition
		core::Pointer<SadBehaviourInstance> sadBehaviourInstance = core::CreatePointer<SadBehaviourInstance>(scriptClass, entity);

		// Store the instantiated behaviour in the lookup
		s_ScriptingData->SadBehaviourInstanceLookup[entity.GetGuid()] = sadBehaviourInstance;

		// Call awake on the script instance
		sadBehaviourInstance->CallAwake();
	}
}

void sad::cs::ScriptingEngine::UpdateSadBehaviourInstance(ecs::Entity entity)
{
	const core::Guid& guid = entity.GetGuid();

	// TODO: If an entity is destroyed in the level - make sure it is killed from this lookup 
	SAD_ASSERT(SadBehaviourInstanceExists(guid), "Attempting to invoke an Update on an entity without a SadBehaviour instance!");

	s_ScriptingData->SadBehaviourInstanceLookup[guid]->CallUpdate();
}

void sad::cs::ScriptingEngine::DestroySadBehaviourInstance(ecs::Entity entity)
{
	const ecs::ScriptComponent& scriptComponent = entity.GetComponent<ecs::ScriptComponent>();
	if (SadBehaviourExists(scriptComponent.m_ClassName))
	{
		s_ScriptingData->SadBehaviourInstanceLookup.erase(entity.GetGuid());
	}
}

////////////////////////////
/// Scripting Engine Ops ///
////////////////////////////

void sad::cs::ScriptingEngine::CacheAssemblySadBehaviours(MonoAssembly* monoAssembly)
{
	s_ScriptingData->SadBehaviourScriptLookup.clear();

	MonoImage* monoImage = mono_assembly_get_image(monoAssembly);
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(monoImage, MONO_TABLE_TYPEDEF);
	int32_t numberOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

	MonoClass* sadBehaviourClass = mono_class_from_name(monoImage, "Sad", "SadBehaviour");

	for (int32_t i = 0; i < numberOfTypes; ++i)
	{
		uint32_t columns[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, columns, MONO_TYPEDEF_SIZE);

		const char* nameSpace = mono_metadata_string_heap(monoImage, columns[MONO_TYPEDEF_NAMESPACE]);
		const char* className = mono_metadata_string_heap(monoImage, columns[MONO_TYPEDEF_NAME]);

		// Evaluate if class in the assembly is inheriting from SadBehaviour
		MonoClass* monoClass = mono_class_from_name(monoImage, nameSpace, className);
		bool isSadBehaviour = mono_class_is_subclass_of(monoClass, sadBehaviourClass, false);

		// Continue to next assembly type if it's not a SadBehaviour or the SadBehaviour class itself
		if (!isSadBehaviour || monoClass == sadBehaviourClass)
			continue;

		// If a valid namespace exists, store it in the format 'NameSpace.ClassName'
		std::string qualifiedName;
		if (strlen(nameSpace) != 0)
		{
			qualifiedName = std::string(nameSpace);
			qualifiedName.append(".");
			qualifiedName.append(className);
		}
		else
		{
			qualifiedName = className;
		}

		s_ScriptingData->SadBehaviourScriptLookup[qualifiedName] = core::CreatePointer<ScriptClass>(nameSpace, className);
		core::Log(ELogType::Info, "[ScriptingEngine] Cached new SadBehaviour with qualified name {}", qualifiedName);
	}
}

MonoObject* sad::cs::ScriptingEngine::InstantiateClass(MonoClass* monoClass)
{
	MonoObject* classInstance = mono_object_new(s_ScriptingData->AppDomain, monoClass);
	SAD_ASSERT(classInstance, "Mono failed to allocate a new object-type for a class");

	// Actually calls the default constructor for HelloWorld
	// Preferable to make the assumption that all SadCSFramework classes will be default constructible
	// If a constructor isn't explicitly constructed at all the C# compiler adds one 
	// Otherwise ensure one exists in the event multiple constructors are used   
	mono_runtime_object_init(classInstance);

	return classInstance;
}

bool sad::cs::ScriptingEngine::SadBehaviourExists(const std::string& qualifiedName)
{
	return s_ScriptingData->SadBehaviourScriptLookup.find(qualifiedName) != s_ScriptingData->SadBehaviourScriptLookup.end();
}

bool sad::cs::ScriptingEngine::SadBehaviourInstanceExists(const core::Guid& guid)
{
	return s_ScriptingData->SadBehaviourInstanceLookup.find(guid) != s_ScriptingData->SadBehaviourInstanceLookup.end();
}

///////////////
/// Testing ///
///////////////

void sad::cs::ScriptingEngine::MonoSanityCheck()
{
	// Create sample TestClass
	ScriptClass testClass = ScriptClass("", "TestClass");
	MonoObject* testObject = testClass.Instantiate();

	// Test 1: Method with no parameters 
	MonoMethod* testMethod = testClass.GetMethod("TestMethod", 0);
	testClass.CallMethod(testMethod, testObject, nullptr);

	// Test 2: Method with one parameter
	//int val = 5;
	//void* param = &val;
	//MonoMethod* incrementMethod = testClass.GetMethod("Increment", 1);
	//testClass.CallMethod(incrementMethod, testObject, &param);

	// Test 3: Method with two parameters
	//int val2 = 10;
	//void* params[2] = { &val, &val2 };
	//MonoMethod* addNumbersMethod = testClass.GetMethod("AddNumbers", 2);
	//testClass.CallMethod(addNumbersMethod, testObject, params);
}

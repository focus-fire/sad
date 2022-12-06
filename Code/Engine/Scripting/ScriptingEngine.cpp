#include "sadpch.h"

#include "ScriptingEngine.h"

#include <mono/metadata/assembly.h>

#include <Engine/Application.h>
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
	std::string sadCSFrameworkAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Project/Resources/SadCSFramework/SadCSFramework.dll");
	LoadSadCSFrameworkAssembly(sadCSFrameworkAssemblyPath);

	// Register components in the engine API
	ScriptingBridge::SetupEngineAPIComponents();

	// Register functions in the engine API
	ScriptingBridge::SetupEngineAPIFunctions();

	// Load the Project assembly with the game's scripts
	std::string projectAssemblyPath = core::FileUtils::GetPathInsideDataDirectory("Project/Resources/SadProject/SadProject.dll");
	std::string fileWatcherPath = core::FileUtils::GetPathInsideDataDirectory("Project/Assets/Scripts");
	LoadProjectAssembly(projectAssemblyPath, fileWatcherPath);

	// Cache classes in assembly
	CacheAssemblySadBehaviours();

	// Hold onto reference of SadBehaviour class in the SadCSFramework image
	s_ScriptingData->SadBehaviourClass = ScriptClass("Sad", "SadBehaviour", true);
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

bool sad::cs::ScriptingEngine::LoadSadCSFrameworkAssembly(const std::string& filePath)
{
	// Create domain for the SadCSFramework runtime
	char appDomainName[] = "SadCSFrameworkScriptRuntime";
	s_ScriptingData->AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(s_ScriptingData->AppDomain, true);

	// Save a reference to the assembly
	s_ScriptingData->SadCSFrameworkAssemblyFilePath = filePath;
	s_ScriptingData->SadCSFrameworkAssembly = ScriptingEngineUtils::LoadCSharpAssembly(filePath);
	if (!s_ScriptingData->SadCSFrameworkAssembly)
		return false;

	// Save a reference to image for the assembly
	s_ScriptingData->SadCSFrameworkImage = mono_assembly_get_image(s_ScriptingData->SadCSFrameworkAssembly);
	return true;
}

bool sad::cs::ScriptingEngine::LoadProjectAssembly(const std::string& assemblyPath, const std::string& fileWatchPath)
{
	s_ScriptingData->ProjectAssemblyFilePath = assemblyPath;
	s_ScriptingData->ProjectAssemblyFileWatchPath = fileWatchPath;
	s_ScriptingData->ProjectAssembly = ScriptingEngineUtils::LoadCSharpAssembly(assemblyPath);
	if (!s_ScriptingData->ProjectAssembly)
		return false;

	s_ScriptingData->ProjectImage = mono_assembly_get_image(s_ScriptingData->ProjectAssembly);

	// Enable file watcher and finish loading assembly
	s_ScriptingData->ProjectAssemblyFileWatcher = core::CreatePointer<filewatch::FileWatch<std::string>>(fileWatchPath, OnProjectAssemblyFileSystemEvent);
	s_ScriptingData->AssemblyReloadInProgress = false;
	return true;
}

void sad::cs::ScriptingEngine::ReloadProjectAssembly()
{
	mono_domain_set(mono_get_root_domain(), false);
	
	mono_domain_unload(s_ScriptingData->AppDomain);

	LoadSadCSFrameworkAssembly(s_ScriptingData->SadCSFrameworkAssemblyFilePath);
	LoadProjectAssembly(s_ScriptingData->ProjectAssemblyFilePath, s_ScriptingData->ProjectAssemblyFileWatchPath);
	CacheAssemblySadBehaviours();

	ScriptingBridge::SetupEngineAPIComponents();

	s_ScriptingData->SadBehaviourClass = ScriptClass("Sad", "SadBehaviour", true);
}

//////////////////
/// Entity Ops ///
//////////////////

void sad::cs::ScriptingEngine::CreateSadBehaviourInstance(ecs::Entity entity, std::string scriptName)
{
	if (!SadBehaviourExists(scriptName))
		return;

	// Retrieve the base class from the lookup
	core::Pointer<ScriptClass> scriptClass = s_ScriptingData->SadBehaviourScriptLookup[scriptName];

	// Instantiate the script using the class definition
	core::Pointer<SadBehaviourInstance> sadBehaviourInstance = core::CreatePointer<SadBehaviourInstance>(scriptClass, entity);

	// Store the instantiated behaviour in the lookup
	s_ScriptingData->SadBehaviourInstanceLookup[entity.GetGuid()] = sadBehaviourInstance;
}

void sad::cs::ScriptingEngine::CreateNativeSadBehaviourInstance(ecs::Entity entity)
{
	const ecs::ScriptComponent& nativeScriptComponent = entity.GetComponent<ecs::ScriptComponent>();

	CreateSadBehaviourInstance(entity, nativeScriptComponent.m_ClassName);
}

void sad::cs::ScriptingEngine::CreateRuntimeSadBehaviourInstance(ecs::Entity entity)
{
	const ecs::RuntimeScriptComponent& runtimeScriptComponent = entity.GetComponent<ecs::RuntimeScriptComponent>();

	CreateSadBehaviourInstance(entity, runtimeScriptComponent.m_ClassName);
}

void sad::cs::ScriptingEngine::AwakeSadBehaviourInstance(ecs::Entity entity, std::string scriptName)
{
	// Exit early if the requested script doesn't exist
	if (!SadBehaviourExists(scriptName))
		return;

	// Retrieve the SadBehaviour from the lookup
	core::Pointer<SadBehaviourInstance> sadBehaviourInstance = s_ScriptingData->SadBehaviourInstanceLookup[entity.GetGuid()];

	// Call awake on the script instance
	sadBehaviourInstance->CallAwake();
}

void sad::cs::ScriptingEngine::AwakeNativeSadBehaviourInstance(ecs::Entity entity)
{
	const ecs::ScriptComponent& nativeScriptComponent = entity.GetComponent<ecs::ScriptComponent>();

	AwakeSadBehaviourInstance(entity, nativeScriptComponent.m_ClassName);
}

void sad::cs::ScriptingEngine::AwakeRuntimeSadBehaviourInstance(ecs::Entity entity)
{
	ecs::RuntimeScriptComponent& runtimeScriptComponent = entity.GetComponent<ecs::RuntimeScriptComponent>();

	AwakeSadBehaviourInstance(entity, runtimeScriptComponent.m_ClassName);
}

void sad::cs::ScriptingEngine::UpdateSadBehaviourInstance(ecs::Entity entity)
{
	const core::Guid& guid = entity.GetGuid();

	SAD_ASSERT(SadBehaviourInstanceExists(guid), "Attempting to invoke an Update on an entity without a SadBehaviour instance");

	s_ScriptingData->SadBehaviourInstanceLookup[guid]->CallUpdate();
}

void sad::cs::ScriptingEngine::DrawGizmosForSadBehaviourInstance(ecs::Entity entity)
{
	const core::Guid& guid = entity.GetGuid();

	SAD_ASSERT(SadBehaviourInstanceExists(guid), "Attempting to invoke an DrawGizmos on an entity without a SadBehaviour instance");

	s_ScriptingData->SadBehaviourInstanceLookup[guid]->CallDrawGizmos();
}

void sad::cs::ScriptingEngine::DestroySadBehaviourInstance(ecs::Entity entity)
{
	bool hasNativeScriptComponent = entity.HasComponent<ecs::ScriptComponent>();
	bool hasRuntimeScriptComponent = entity.HasComponent<ecs::RuntimeScriptComponent>();

	// Entity passed doesn't have a script to destroy
	if (!hasNativeScriptComponent && !hasRuntimeScriptComponent)
		return;

	if (hasNativeScriptComponent)
	{
		const ecs::ScriptComponent& nativeScriptComponent = entity.GetComponent<ecs::ScriptComponent>();
		if (SadBehaviourExists(nativeScriptComponent.m_ClassName))
			s_ScriptingData->SadBehaviourInstanceLookup.erase(entity.GetGuid());

		entity.RemoveComponent<ecs::ScriptComponent>();
	}

	if (hasRuntimeScriptComponent)
	{
		const ecs::RuntimeScriptComponent& runtimeScriptComponent = entity.GetComponent<ecs::RuntimeScriptComponent>();
		if (SadBehaviourExists(runtimeScriptComponent.m_ClassName))
			s_ScriptingData->SadBehaviourInstanceLookup.erase(entity.GetGuid());

		entity.RemoveComponent<ecs::RuntimeScriptComponent>();
	}
}

//////////////////////
/// Exposed C# Ops ///
//////////////////////

MonoObject* sad::cs::ScriptingEngine::GetSadBehaviourInstance(const core::Guid& guid)
{
	if (!SadBehaviourInstanceExists(guid))
	{
		core::Log(ELogType::Warn, "[ScriptingEngine] A script is trying to get a script instance that doesn't exist");
		return nullptr;
	}

	return s_ScriptingData->SadBehaviourInstanceLookup.at(guid)->GetManagedInstance();
}

void sad::cs::ScriptingEngine::AddRuntimeSadBehaviourInstance(ecs::Entity entity, std::string scriptName)
{
	// Check if the entity already has an instantiated script attached to it
	if (SadBehaviourInstanceExists(entity.GetGuid()))
	{
		core::Log(ELogType::Warn, "[ScriptingEngine] A script is trying to add a script to an entity that already has one");
		return;
	}

	// Check if script exists in the runtime
	if (!SadBehaviourExists(scriptName))
	{
		core::Log(ELogType::Warn, "[ScriptingEngine] A script is trying to add a script to an entity, but the script doesn't exist");
		return;
	}

	// Add the component to the entity
	entity.AddComponent<ecs::RuntimeScriptComponent>(scriptName);

	// Create the runtime instance
	CreateRuntimeSadBehaviourInstance(entity);

	// Awaken the script since this is called from runtime
	AwakeRuntimeSadBehaviourInstance(entity);
}

////////////////////////////
/// Scripting Engine Ops ///
////////////////////////////

void sad::cs::ScriptingEngine::CacheAssemblySadBehaviours()
{
	s_ScriptingData->SadBehaviourScriptLookup.clear();
	s_ScriptingData->SadBehaviourInstanceLookup.clear();

	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_ScriptingData->ProjectImage, MONO_TABLE_TYPEDEF);
	int32_t numberOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

	// Retrieve Sad.SadBehaviour from the SadCSFramework internal API 
	MonoClass* sadBehaviourClass = mono_class_from_name(s_ScriptingData->SadCSFrameworkImage, "Sad", "SadBehaviour");

	for (int32_t i = 0; i < numberOfTypes; ++i)
	{
		uint32_t columns[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, columns, MONO_TYPEDEF_SIZE);

		const char* nameSpace = mono_metadata_string_heap(s_ScriptingData->ProjectImage, columns[MONO_TYPEDEF_NAMESPACE]);
		const char* className = mono_metadata_string_heap(s_ScriptingData->ProjectImage, columns[MONO_TYPEDEF_NAME]);

		// Evaluate if class in the assembly is inheriting from SadBehaviour
		MonoClass* monoClass = mono_class_from_name(s_ScriptingData->ProjectImage, nameSpace, className);
		bool isSadBehaviour = mono_class_is_subclass_of(monoClass, sadBehaviourClass, false);

		// Continue to next assembly type if it's not a SadBehaviour or the SadBehaviour class itself
		if (!isSadBehaviour || monoClass == sadBehaviourClass)
			continue;

		// If a valid namespace exists, store it in the format 'NameSpace.ClassName'
		std::string qualifiedName;
		if (strlen(nameSpace) != 0)
			qualifiedName = fmt::format("{}.{}", nameSpace, className);
		else
			qualifiedName = className;

		s_ScriptingData->SadBehaviourScriptLookup[qualifiedName] = core::CreatePointer<ScriptClass>(nameSpace, className);
		core::Log(ELogType::Trace, "[ScriptingEngine] Cached new SadBehaviour with qualified name {}", qualifiedName);
	}
}


void sad::cs::ScriptingEngine::OnProjectAssemblyFileSystemEvent(const std::string& filePath, const filewatch::Event eventType)
{
	if (!s_ScriptingData->AssemblyReloadInProgress && eventType == filewatch::Event::modified)
	{
		s_ScriptingData->AssemblyReloadInProgress = true;
		
		Application::s_ThreadQueue->SubmitToApplicationThreadQueue([]()
		{
			core::Log(ELogType::Info, "Detected script changes, reloading assemblies");
			s_ScriptingData->ProjectAssemblyFileWatcher.reset();
			ScriptingEngine::ReloadProjectAssembly();
		});
	}
}

MonoObject* sad::cs::ScriptingEngine::InstantiateClass(MonoClass* monoClass)
{
	MonoObject* classInstance = mono_object_new(s_ScriptingData->AppDomain, monoClass);
	SAD_ASSERT(classInstance, "Mono failed to allocate a new object-type for a class");

	// Actually calls the default constructor 
	// Preferable to make the assumption that all SadCSFramework classes will be default constructible
	// If a constructor isn't explicitly constructed at all the C# compiler adds one 
	// Otherwise ensure one exists in the event multiple constructors are used   
	mono_runtime_object_init(classInstance);

	return classInstance;
}

bool sad::cs::ScriptingEngine::SadBehaviourExists(const std::string& qualifiedName)
{
	return s_ScriptingData->SadBehaviourScriptLookup.contains(qualifiedName);
}

bool sad::cs::ScriptingEngine::SadBehaviourInstanceExists(const core::Guid& guid)
{
	return s_ScriptingData->SadBehaviourInstanceLookup.contains(guid);
}

bool sad::cs::ScriptingEngine::SadBehaviourInstanceExists(const core::Guid& guid, const std::string& qualifiedName)
{
	if (!SadBehaviourInstanceExists(guid))
		return false;

	// Check if the passed script name and the script name on the SadBehaviour are the same
	ScriptClass* sadBeaviourDefinition = s_ScriptingData->SadBehaviourInstanceLookup[guid]->GetScriptDefinition();
	std::string retrievedQualifiedName = sadBeaviourDefinition->GetQualifiedName();

	return core::StringUtils::Equals(qualifiedName, retrievedQualifiedName);
}

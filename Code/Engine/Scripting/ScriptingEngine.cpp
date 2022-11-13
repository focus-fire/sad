#include "sadpch.h"

#include "ScriptingEngine.h"

#include <mono/metadata/assembly.h>

void sad::cs::ScriptingEngine::Start()
{
	m_ScriptingEngineConfig = new ScriptingEngineConfig();
	
	StartMono();
}

void sad::cs::ScriptingEngine::Teardown()
{
	delete m_ScriptingEngineConfig;
}

void sad::cs::ScriptingEngine::StartMono()
{
	// Sets path for cs assemblies relative to the current working directory
	// This could be set as a config variable and modified by engine users (ie: if a local set of assemblies is installed)
	// For the scope of this project though it probably makes sense to leave it in the project
	mono_set_assemblies_path("SadScripting/.net/lib");

	// Create root domain for JIT runtime
	MonoDomain* csRootDomain = mono_jit_init("sadJITRuntime");
	SAD_ASSERT(csRootDomain, "Failed to initialize Mono JIT runtime");
	m_ScriptingEngineConfig->RootDomain = csRootDomain;

	// Create app domain for Mono
	char appDomainName[] = "sadScriptRuntime";
	m_ScriptingEngineConfig->AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(m_ScriptingEngineConfig->AppDomain, true);
}

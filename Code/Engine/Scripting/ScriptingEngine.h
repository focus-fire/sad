#pragma once

#include <unordered_map>

#include <mono/jit/jit.h>

#include <Core/Memory.h>
#include <Engine/Level.h>

#include "ScriptClass.h"
#include "SadBehaviourInstance.h"

namespace sad::cs
{
	/**
	 * @brief Handles the initialization, loading/unloading, and teardown of scripting assemblies
	*/
	class ScriptingEngine
	{
	private:
		/**
		 * @brief Struct containing data required to perform mono operations throughout the scripting engine
		*/
		struct ScriptingEngineData
		{
			MonoDomain* RootDomain = nullptr;
			MonoDomain* AppDomain = nullptr;
			
			std::string SadCSFrameworkAssemblyFilePath;
			MonoAssembly* SadCSFrameworkAssembly = nullptr;
			MonoImage* SadCSFrameworkImage = nullptr;

			std::string ProjectAssemblyFilePath;
			MonoAssembly* ProjectAssembly = nullptr;
			MonoImage* ProjectImage = nullptr;

			// TODO: Is mapping required here? Sets?
			std::unordered_map<std::string, core::Pointer<ScriptClass>> SadBehaviourScriptLookup;
			std::unordered_map<core::Guid, core::Pointer<SadBehaviourInstance>> SadBehaviourInstanceLookup;

			Level* CurrentLevelInstance = nullptr;
		};

	public:
		static void Start();
		static void Teardown();

		static void RuntimeStart(Level* level);
		static void RuntimeStop();

		static void AwakeSadBehaviourInstance(ecs::Entity entity);
		static void UpdateSadBehaviourInstance(ecs::Entity entity);

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static bool SadBehaviourExists(const std::string& qualifiedName);
		static bool SadBehaviourInstanceExists(const core::Guid& guid);

		static Level* GetCurrentLevelInstance() { return s_ScriptingData->CurrentLevelInstance; }

	public:
		static ScriptingEngineData* s_ScriptingData;

	private:
		static void StartMono();
		static void TeardownMono();

		/**
		 * @brief Loads the assembly for the SadCSFramework into the scripting engine
		 * @param filePath Path to the .dll with the SadCSFramework binaries
		*/
		static void LoadSadCSFrameworkAssembly(const std::string& filePath);

		/**
		 * @brief Loads the assembly for the current scripting project into the scripting engine
		 * @param filePath Path to the .dll with the project's binaries 
		*/
		static void LoadProjectAssembly(const std::string& filePath);

		/**
		 * @brief Caches all SadBehaviours detected in target asesmbly into the scripting engine's ScriptLookup 
		 * @todo Add PrintAssemblySadBehaviours to the cli?
		*/
		static void CacheAssemblySadBehaviours(MonoAssembly* monoAssembly);

		// TEMP
		static void MonoSanityCheck();
	};
}

#pragma once

#include <mono/jit/jit.h>

#include <Engine/Level.h>

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
		struct ScriptingEngineConfig
		{
			MonoDomain* RootDomain = nullptr;
			MonoDomain* AppDomain = nullptr;
			
			std::string SadCSFrameworkAssemblyFilePath;
			MonoAssembly* SadCSFrameworkAssembly = nullptr;
			MonoImage* SadCSFrameworkImage = nullptr;

			std::string ProjectAssemblyFilePath;
			MonoAssembly* ProjectAssembly = nullptr;
			MonoImage* ProjectImage = nullptr;

			Level* CurrentLevelInstance = nullptr;
		};

	public:
		static void Start();
		static void Teardown();

		static void RuntimeStart(Level* level);
		static void RuntimeStop();

		static Level* GetCurrentLevelInstance() { return s_ScriptingConfig->CurrentLevelInstance; }

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

		// UTIL
		static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		static MonoObject* InstantiateClass(const char* namespaceName, const char* className);

		// TEMP
		static void CallTestMethod(MonoObject* objectInstance);
		static void CallIncrementTestMethod(MonoObject* objectInstance, int value);
		static void CallStringTestMethod(MonoObject* objectInstance, const char* str);

		static ScriptingEngineConfig* s_ScriptingConfig;
	};
}

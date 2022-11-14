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
		struct ScriptingEngineConfig
		{
			MonoDomain* RootDomain = nullptr;
			MonoDomain* AppDomain = nullptr;
			MonoAssembly* SadCSFrameworkAssembly = nullptr;

			Level* CurrentLevelInstance = nullptr;
		};

	public:
		static void Start();
		static void Teardown();

		static void RuntimeStart(Level* level);
		static void RuntimeStop();

	private:
		static void StartMono();
		static void TeardownMono();

		static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		static MonoObject* InstantiateClass(const char* namespaceName, const char* className);

		// TEMP
		static void CallTestMethod(MonoObject* objectInstance);
		static void CallIncrementTestMethod(MonoObject* objectInstance, int value);
		static void CallStringTestMethod(MonoObject* objectInstance, const char* str);

		static ScriptingEngineConfig* s_ScriptingConfig;
	};
}

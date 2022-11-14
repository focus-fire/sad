#pragma once

#include <mono/jit/jit.h>

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
		};

	public:
		void Start();
		void Teardown();

	private:
		void StartMono();
		void TeardownMono();

		char* ReadBytes(const std::string& filePath, uint32_t* outputSize);
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		void PrintAssemblyTypes(MonoAssembly* assembly);
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);

		// TEMP
		MonoObject* InstantiateClass(const char* namespaceName, const char* className);
		void CallTestMethod(MonoObject* objectInstance);
		void CallIncrementTestMethod(MonoObject* objectInstance, int value);

		ScriptingEngineConfig* m_ScriptingConfig;
	};
}

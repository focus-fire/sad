#pragma once

#include <mono/jit/jit.h>

namespace sad::cs
{
	class ScriptingEngine
	{
	private:
		struct ScriptingEngineConfig
		{
			MonoDomain* RootDomain = nullptr;
			MonoDomain* AppDomain = nullptr;
		};

	public:
		void Start();
		void Teardown();

	private:
		void StartMono();

		ScriptingEngineConfig* m_ScriptingEngineConfig;
	};
}

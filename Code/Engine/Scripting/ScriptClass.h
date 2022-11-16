#pragma once

#include <string>

#include <mono/metadata/metadata.h>

namespace sad::cs
{
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& methodName, int paramCount);
		MonoObject* CallMethod(MonoMethod* method, MonoObject* instance, void* params[] = nullptr);

	private:
		std::string m_NameSpace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};
}

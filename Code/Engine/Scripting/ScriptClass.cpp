#include "sadpch.h"

#include "ScriptClass.h"

#include "ScriptingEngine.h"

sad::cs::ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& className, bool useSadCSFrameworkImage)
	: m_NameSpace(nameSpace)
	, m_ClassName(className)
{
	if (useSadCSFrameworkImage)
		m_MonoClass = mono_class_from_name(ScriptingEngine::s_ScriptingData->SadCSFrameworkImage, nameSpace.c_str(), className.c_str());
	else
		m_MonoClass = mono_class_from_name(ScriptingEngine::s_ScriptingData->ProjectImage, nameSpace.c_str(), className.c_str());

	SAD_ASSERT(m_MonoClass, "Mono failed to allocate a new object-type for a class");
}

MonoObject* sad::cs::ScriptClass::Instantiate()
{
	MonoObject* classInstance = ScriptingEngine::InstantiateClass(m_MonoClass);
	SAD_ASSERT(classInstance, "Failed to instantiate Mono class");

	return classInstance;
}

MonoMethod* sad::cs::ScriptClass::GetMethod(const std::string& methodName, int paramCount)
{
	return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), paramCount);
}

MonoObject* sad::cs::ScriptClass::CallMethod(MonoMethod* method, MonoObject* instance, void** params /* = nullptr */)
{
	MonoObject* exception = nullptr;
	MonoObject* result = mono_runtime_invoke(method, instance, params, &exception);
	SAD_ASSERT(!exception, "Unhandled exception occurred while executing method from CS assembly");

	return result;
}

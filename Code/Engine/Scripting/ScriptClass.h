#pragma once

#include <string>

#include <mono/metadata/metadata.h>

namespace sad::cs
{
	/**
	 * @brief The literal 'class' or 'file' representation of a C# script.
	 *		  Mainly a container for the metadata Mono exposes for each script.
	*/
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className);

		/**
		 * @brief Instantiates an instance of this script's class in Mono
		 * @return Pointer to the object for this script in the Mono runtime 
		*/
		MonoObject* Instantiate();

		/**
		 * @brief Attempts to retrieve a method from the given script 
		 * @param methodName Name of the method for retrieval
		 * @param paramCount Number of parameters in the signature for the method
		 * @return Pointer to the method contained in the script and in the Mono runtime
		 * @note This method will return 'nullptr' if the method requested does not exist on the file.
		 *	     Doing this allows scripts to situationally have methods - such as 'Awake' or 'Update'.
		*/
		MonoMethod* GetMethod(const std::string& methodName, int paramCount);

		/**
		 * @brief Calls or ('invokes') a method in script
		 * @param method Pointer to the method being invoked in the Mono runtime
		 * @param instance Instance of the object required to invoke the method on
		 * @param params Ambiguous array of params required to fill the method's signature
		 * @return Pointer to an object in Mono containing the result of the method call
		 * @note Calling this method will assert if the C# runtime throws an exception
		*/
		MonoObject* CallMethod(MonoMethod* method, MonoObject* instance, void** params = nullptr);

	private:
		std::string m_NameSpace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};
}

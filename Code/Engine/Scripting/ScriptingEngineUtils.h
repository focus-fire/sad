#pragma once

#include <string>

#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>

#include "ScriptableTypes.h"

namespace sad::cs
{
	/**
	 * @brief Utilities used for driving the scripting engine and Mono
	*/
	class ScriptingEngineUtils
	{
	public:
		/**
		 * @brief Loads a CSharp assembly (.dll) at a given path
		 * @param assemblyPath Path to the assembly required for loading
		 * @return Raw pointer generated by Mono representing the assembly metadata and information
		*/
		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		
		/**
		 * @brief Prints assembly types to the log
		 * @param assembly Raw pointer to assembly in Mono
		*/
		static void PrintAssemblyTypes(MonoAssembly* assembly);

		/**
		 * @brief Converts from a mono type to a pre-defined scriptable type
		 * @param type The mono type to make a conversion against
		 * @return Enum containing the evaluated ScriptableType
		*/
		static ScriptableType MonoTypeToScriptableType(MonoType* type);
	};
}
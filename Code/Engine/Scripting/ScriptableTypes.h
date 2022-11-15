#pragma once

#include <unordered_map>

#include <Core/StringUtils.h>

namespace sad::cs
{
	/**
	 * @brief Types elgible for conversion between the Mono runtime and the sad engine 
	*/
	enum class ScriptableType
	{
		None,
		Int,
		Float,
		Double,
		Bool,
		Char,

		Vector3,
		Entity
	};

	/**
	 * @brief Lookup for type definitions between the SadCSFramework and internal API 
	*/
	static std::unordered_map<std::string, ScriptableType> s_MonoScriptableTypeLookup =
	{
		{ "System.Int32", ScriptableType::Int },
		{ "System.Single", ScriptableType::Float },
		{ "System.Double", ScriptableType::Double },
		{ "System.Boolean", ScriptableType::Bool },
		{ "System.Char", ScriptableType::Char },

		{ "Sad.Vector3", ScriptableType::Vector3 },
		{ "Sad.Entity", ScriptableType::Entity }
	};

	/**
	 * @brief Retrieves the string representation for a ScriptableType
	 * @param type Type to retrieve the string representation of
	 * @return String containing the value of the enum
	*/
	inline std::string ScriptableTypeToString(const ScriptableType& type)
	{
		switch (type)
		{
		case ScriptableType::None:
			return "None";
		case ScriptableType::Int: 
			return "Int";
		case ScriptableType::Float: 
			return "Float";
		case ScriptableType::Double:
			return "Double";
		case ScriptableType::Bool:
			return "Bool";
		case ScriptableType::Char:
			return "Char";
		case ScriptableType::Vector3:
			return "Vector3";
		case ScriptableType::Entity:
			return "Entity";
		}
		
		core::Log(ELogType::Error, "[ScriptableTypes] Attempted to convert unmapped scriptable type to string");

		return "None";
	}

	/**
	 * @brief Converts a provided string to a ScriptableType enum
	 * @param type String containing the value to convert to a ScriptableType
	 * @return Enum representing the string as a ScriptableType
	*/
	inline ScriptableType ScriptableTypeFromString(const std::string& type)
	{
		using String = core::StringUtils;

		if (String::Equals(type, "None"))
			return ScriptableType::None;

		if (String::Equals(type, "Int"))
			return ScriptableType::Int;

		if (String::Equals(type, "Float"))
			return ScriptableType::Float;

		if (String::Equals(type, "Double"))
			return ScriptableType::Double;

		if (String::Equals(type, "Bool"))
			return ScriptableType::Bool;

		if (String::Equals(type, "Char"))
			return ScriptableType::Char;

		if (String::Equals(type, "Vector3"))
			return ScriptableType::Vector3;
		
		if (String::Equals(type, "Entity"))
			return ScriptableType::Entity;

		core::Log(ELogType::Error, "[ScriptableTypes] Attempted to retrieve enum for unmapped scriptable type {}", type);

		return ScriptableType::None;
	}
}

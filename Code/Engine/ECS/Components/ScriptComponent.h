#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace sad::ecs
{
	/**
	 * @brief Used for attaching custom C# scripts with a SadBehaviour to entities in the level 
	*/
	struct ScriptComponent
	{
		std::string m_ClassName;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::ScriptComponent& scriptComponent)
	{
		std::string className = scriptComponent.m_ClassName;

		JSON =
		{
			{
				"ClassName", className 
			},
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::ScriptComponent& scriptComponent)
	{
		scriptComponent.m_ClassName = JSON["ClassName"];
	}
}

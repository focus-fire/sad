#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace sad::ecs
{
	/**
	 * @brief Identifies entities with a human-readable identifier 
	*/
	struct NameComponent
	{
		std::string m_Name;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::NameComponent& nameComponent)
	{
		std::string name = nameComponent.m_Name;

		JSON =
		{
			{
				"Name", name
			},
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::NameComponent& nameComponent)
	{
		nameComponent.m_Name = JSON["Name"];
	}
}

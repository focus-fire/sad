#pragma once

#include <nlohmann/json.hpp>

#include <Core/Guid.h>

namespace sad::ecs
{
	/**
	 * @brief Component that creates a GUID upon construction, used to identify entities
	*/
	struct GuidComponent
	{
		core::Guid m_GUID;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::GuidComponent& guidComponent)
	{
		std::string guid = guidComponent.m_GUID.ToString();

		JSON =
		{
			{
				"GUID", guid
			},
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::GuidComponent& guidComponent)
	{
		std::string guid = JSON["GUID"];
		
		// Automatically create a guid for entities on reimport if they don't have one
		if (!guid.empty())
			guidComponent.m_GUID = core::Guid::RecreateGuid(JSON["GUID"]);
		else
			guidComponent.m_GUID = core::Guid::CreateGuid();
	}
}

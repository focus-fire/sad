#pragma once

#include <Engine/PointLight.h>

namespace sad::ecs
{
	/**
	 * @brief It is a light
	*/
	struct PointLightComponent
	{
		PointLight m_Light;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::PointLightComponent& pointLight)
	{
		glm::vec3 pos = pointLight.m_Light.Position;

		JSON =
		{
			{
				"PointLightPosition",
				{
					{"x", pos.x},
					{"y", pos.y},
					{"z", pos.z}
				}
			},
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::PointLightComponent& pointLight)
	{
		glm::vec3 pointLightPosition = glm::vec3(JSON["PointLightPosition"]["x"], JSON["PointLightPosition"]["y"], JSON["PointLightPosition"]["z"]);
		pointLight.m_Light = PointLight(pointLightPosition);
	}
}

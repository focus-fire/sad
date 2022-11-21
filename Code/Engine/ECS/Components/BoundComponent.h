#pragma once

#include <Engine/Bound.h>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the Bound of a particular sad::Entity
	*/
	struct BoundComponent
	{
		core::Pointer<Bound> m_Bound;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::BoundComponent& boundComponent)
	{
		glm::vec3 center = boundComponent.m_Bound->GetCenter();
		glm::vec3 size = boundComponent.m_Bound->GetSize();

		JSON =
		{
			{
				"BoundCenter",
				{
					{"x", center.x},
					{"y", center.y},
					{"z", center.z}
				}
			},
			{
				"BoundSize",
				{
					{"x", size.x},
					{"y", size.y},
					{"z", size.z}
				}
			}
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::BoundComponent& boundComponent)
	{
		glm::vec3 center = glm::vec3();
		glm::vec3 size = glm::vec3();

		center = glm::vec3(JSON["BoundCenter"]["x"], JSON["BoundCenter"]["y"], JSON["BoundCenter"]["z"]);
		size = glm::vec3(JSON["BoundSize"]["x"], JSON["BoundSize"]["y"], JSON["BoundSize"]["z"]);

		boundComponent.m_Bound = core::CreatePointer<Bound>(center, size);
	}
}
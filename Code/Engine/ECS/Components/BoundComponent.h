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

	// TODO: Bounds converted to/and from json might not be functioning properly
	inline void to_json(nlohmann::json& JSON, const sad::ecs::BoundComponent& boundComponent)
	{
		glm::vec3 pos = boundComponent.m_Bound->GetPosition();
		glm::vec3 scl = boundComponent.m_Bound->GetScale();

		JSON =
		{
			{
				"boundPosition",
				{
					{"x", pos.x},
					{"y", pos.y},
					{"z", pos.z}
				}
			},
			{
				"boundScale",
				{
					{"x", scl.x},
					{"y", scl.y},
					{"z", scl.z}
				}
			}
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::BoundComponent& boundComponent)
	{
		glm::vec3 pos = glm::vec3();
		glm::vec3 scl = glm::vec3();

		pos = glm::vec3(JSON["boundPosition"]["x"], JSON["boundPosition"]["y"], JSON["boundPosition"]["z"]);
		scl = glm::vec3(JSON["boundScale"]["x"], JSON["boundScale"]["y"], JSON["boundScale"]["z"]);

		boundComponent.m_Bound = core::CreatePointer<Bound>(pos, scl);
	}
}
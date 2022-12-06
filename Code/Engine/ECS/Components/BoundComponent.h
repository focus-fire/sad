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
		glm::vec3 sizeDiff = boundComponent.m_Bound->GetSizeDiff();
		glm::quat rotation = boundComponent.m_Bound->GetRotation();

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
					{"x", size.x == 0.0f ? 0.0f : size.x },
					{"y", size.y == 0.0f ? 0.0f : size.y },
					{"z", size.z == 0.0f ? 0.0f : size.z }
				}
			},
			{
				"BoundSizeDiff",
				{
					{"x", sizeDiff.x},
					{"y", sizeDiff.y},
					{"z", sizeDiff.z},
				}
			},
			{
				"BoundRotation",
				{
					{"x", rotation.x},
					{"y", rotation.y},
					{"z", rotation.z},
					{"w", rotation.w},
				}
			}
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::BoundComponent& boundComponent)
	{
		glm::vec3 center = glm::vec3();
		glm::vec3 size = glm::vec3();
		glm::vec3 sizeDiff = glm::vec3();
		glm::quat rotation = glm::quat();

		center = glm::vec3(JSON["BoundCenter"]["x"], JSON["BoundCenter"]["y"], JSON["BoundCenter"]["z"]);
		size = glm::vec3(JSON["BoundSize"]["x"], JSON["BoundSize"]["y"], JSON["BoundSize"]["z"]);
		sizeDiff = glm::vec3(JSON["BoundSizeDiff"]["x"], JSON["BoundSizeDiff"]["y"], JSON["BoundSizeDiff"]["z"]);
		rotation = glm::quat(JSON["BoundRotation"]["x"], JSON["BoundRotation"]["y"], JSON["BoundRotation"]["z"], JSON["BoundRotation"]["w"]);

		//boundComponent.m_Bound = core::CreatePointer<Bound>(center, size);
		boundComponent.m_Bound = core::CreatePointer<Bound>(center, size, sizeDiff, rotation);
	}
}
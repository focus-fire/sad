<<<<<<< HEAD
#pragma once

#include <Engine/Transform.h>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the Transform of a particular sad::Entity
	 *
	 * @note As most TransformComponents should be manipulated by entities themselves, there
	 *       shouldn't be a corresponding TransformSystem or other systems attempting to modify them.
	*/
	struct TransformComponent
	{
		Transform* m_Transform;
	};
}
=======
#pragma once

#include <Engine/Transform.h>
#include <nlohmann/json.hpp>

namespace sad::ecs
{
	/**
	 * @brief Component holding a reference to the Transform of a particular sad::Entity
	 *
	 * @note As most TransformComponents should be manipulated by entities themselves, there
	 *       shouldn't be a corresponding TransformSystem or other systems attempting to modify them.
	*/
	struct TransformComponent
	{
		core::Pointer<Transform> m_Transform;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::TransformComponent& transformComponent)
	{
		glm::vec3 pos = transformComponent.m_Transform->GetPosition();
		glm::quat rot = transformComponent.m_Transform->GetRotation();
		glm::vec3 scl = transformComponent.m_Transform->GetScale();

		JSON =
		{
			{
				"position",
				{
					{"x", pos.x},
					{"y", pos.y},
					{"z", pos.z}
				}
			},
			{
				"rotation",
				{
					{"w", rot.w},
					{"x", rot.x},
					{"y", rot.y},
					{"z", rot.z}
				}
			},
			{
				"scale",
				{
					{"x", scl.x},
					{"y", scl.y},
					{"z", scl.z}
				}
			}
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::TransformComponent& transformComponent)
	{
		glm::vec3 pos = glm::vec3();
		glm::quat rot = glm::quat();
		glm::vec3 scl = glm::vec3();

		
		pos = glm::vec3(JSON["position"]["x"], JSON["position"]["y"], JSON["position"]["z"]);
		rot = glm::quat(JSON["rotation"]["w"], JSON["rotation"]["x"], JSON["rotation"]["y"], JSON["rotation"]["z"]);
		scl = glm::vec3(JSON["scale"]["x"], JSON["scale"]["y"], JSON["scale"]["z"]);

		transformComponent.m_Transform = core::CreatePointer<Transform>();
		transformComponent.m_Transform->SetPosition(pos);
		transformComponent.m_Transform->SetRotation(rot);
		transformComponent.m_Transform->SetScale(scl);
	}
}
>>>>>>> dev

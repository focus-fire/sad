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

	inline void to_json(nlohmann::json& j, const sad::ecs::TransformComponent& p)
	{
		glm::vec3 pos = p.m_Transform->GetPosition();
		glm::quat rot = p.m_Transform->GetRotation();
		glm::vec3 scl = p.m_Transform->GetScale();

		j =
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

	inline void from_json(const nlohmann::json& j, sad::ecs::TransformComponent& p)
	{
		glm::vec3 pos = glm::vec3();
		glm::quat rot = glm::quat();
		glm::vec3 scl = glm::vec3();

		
		pos = glm::vec3(j["position"]["x"], j["position"]["y"], j["position"]["z"]);
		rot = glm::quat(j["rotation"]["w"], j["rotation"]["x"], j["rotation"]["y"], j["rotation"]["z"]);
		scl = glm::vec3(j["scale"]["x"], j["scale"]["y"], j["scale"]["z"]);

		p.m_Transform = core::CreatePointer<Transform>();
		p.m_Transform->SetPosition(pos);
		p.m_Transform->SetRotation(rot);
		p.m_Transform->SetScale(scl);
	}
}

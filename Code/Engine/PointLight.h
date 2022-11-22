#pragma once

#include <glm/glm.hpp>

namespace sad
{
	class PointLight
	{
	public:
		PointLight()
			: Position(0.0f)
		{ }

		PointLight(glm::vec3 position)
			: Position(position)
		{ }

		~PointLight() = default;

		glm::vec3 Position;
	};
}

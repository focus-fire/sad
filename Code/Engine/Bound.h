#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ECS/Components/TransformComponent.h"

namespace sad 
{
	class Bound 
	{
	public:
		/*
		* Find entity and use transform data 
		* to get position to create bounds.
		*/
		Bound(glm::vec3 position, glm::vec3 scale);
		Bound(const sad::ecs::TransformComponent transform);
		~Bound();

		/*
		* Value should be auto set to the transform position, however, if
		* user would like to adjust the bound to have its own unique position / scale
		* This allows for user to do so.
		*/
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }

		/*
		* Bound Max and Min calculations.
		*/
		void CalculateBound(glm::vec3 position, glm::vec3 scale);

		// Haven't set rotation yet
		// void SetRotation(glm::quat rotation) { m_Rotation = rotation; }

		/*
		* Get position and scale values.
		*/
		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetScale() { return m_Scale; }
		glm::vec3 GetBoundMax() { return m_Max; }
		glm::vec3 GetBoundMin() { return m_Min; }

	private:

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		// glm::quat m_Rotation;

		/*
		Max and Min vectors for a bounds box.
		*/
		glm::vec3 m_Max;
		glm::vec3 m_Min;
	};
}
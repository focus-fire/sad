#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ECS/Components/TransformComponent.h"
#include "Transform.h"

#include "ECS/Components/BoundComponent.h"

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
		Bound(const sad::ecs::TransformComponent transformComponent);
		~Bound();

		/*
		* Value should be auto set to the transform position.
		*/
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }
		// Haven't set rotation yet - this might be needed but could make it more complex
		// void SetRotation(glm::quat rotation) { m_Rotation = rotation; }

		/*
		* Bound Max and Min calculations.
		*/
		void CalculateBound(glm::vec3 position, glm::vec3 scale);

		/*
		* Checks for bound intersection with 
		* another pair of MaxMin vectors.
		*/
		bool Intersects(glm::vec3 otherMax, glm::vec3 otherMin);
		
		/* 
		* Checks for bound Intersection after 
		* pulling MaxMin vectors from compoenent..
		*/
		bool Intersects(sad::ecs::BoundComponent otherBound);

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

		/*
		* Checks another bound's max and min vectors to check for overlap or intersection.
		*/
		bool IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin);
	};
}
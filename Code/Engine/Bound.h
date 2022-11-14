#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Transform.h"

namespace sad 
{
	class Bound 
	{
	public:
		/*
		* Find entity and use transform data 
		* to get position to create bounds.
		*/
		explicit Bound(glm::vec3 position, glm::vec3 scale);
		explicit Bound(Transform transform);

		/*
		* Value should be auto set to the transform position.
		*/
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }

		/*
		* Bound Max and Min calculations.
		*/
		void CalculateBound();

		/*
		* Checks for bound intersection with 
		* another pair of MaxMin vectors.
		*/
		bool Intersects(glm::vec3 otherMax, glm::vec3 otherMin);
		
		/* 
		* Checks for bound Intersection after 
		* pulling MaxMin vectors from compoenent..
		*/
		bool Intersects(Bound otherBound);

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
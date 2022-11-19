#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Transform.h"

namespace sad 
{
	/**
	 * @brief Basic AABB Bound, attached to most objects as a simple reference-point for collision detection
	*/
	class Bound 
	{
	public:
		explicit Bound(glm::vec3 center, glm::vec3 size);
		explicit Bound(Transform transform);

		/**
		 * @brief Checks for bound intersection with another pair of max and min vectors
		*/
		bool Intersects(glm::vec3 otherMax, glm::vec3 otherMin);
		
		/** 
		 * @brief Checks for bound Intersection after pulling max and min vectors from bound
		*/
		bool Intersects(Bound otherBound);

		glm::vec3 GetCenter() { return m_Center; }
		void SetCenter(glm::vec3 center) { m_Center = center; }

		glm::vec3 GetSize() { return m_Extents * 2.0f; }
		void SetSize(glm::vec3 size) { m_Extents = size * 0.5f; }

		void SetMinMax(glm::vec3 min, glm::vec3 max);
		glm::vec3 GetBoundMax() { return m_Center + m_Extents; }
		glm::vec3 GetBoundMin() { return m_Center - m_Extents; }

	private:
		/**
		 * @brief Checks another bound's max and min vectors to check for overlap or intersection.
		*/
		bool IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin);

	private:
		glm::vec3 m_Center;
		glm::vec3 m_Extents;
		glm::vec3 m_Size;
	};
}
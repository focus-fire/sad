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
		explicit Bound(glm::vec3 center, glm::vec3 size, glm::vec3 sizeDiff, glm::quat rot);
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

		glm::vec3 GetSize() { return (m_Extents * 2.0f) * m_SizeDifference; }
		void SetSize(glm::vec3 size) { m_Extents = (size / m_SizeDifference) * 0.5f; }
		void SetSizeRatio(glm::vec3 scale);

		glm::quat GetRotation() { return m_rotation; }
		void SetRotation(glm::quat rotation) { m_rotation = rotation; }

		void SetMinMax(glm::vec3 min, glm::vec3 max);
		glm::vec3 GetBoundMax() { return m_Center + m_Extents; }
		glm::vec3 GetBoundMin() { return m_Center - m_Extents; }

		glm::vec3 GetSizeDiff() { return m_SizeDifference; }
		glm::vec3 SetSizeDiff(glm::vec3 sizeDiff) { return m_SizeDifference = sizeDiff; }

	private:
		/**
		 * @brief Checks another bound's max and min vectors to check for overlap or intersection.
		*/
		bool IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin);

	private:
		glm::vec3 m_Center;
		glm::vec3 m_Extents;
		glm::vec3 m_Size;
		glm::quat m_rotation;
		glm::vec3 m_SizeDifference = glm::vec3(0, 0, 0);
	};
}
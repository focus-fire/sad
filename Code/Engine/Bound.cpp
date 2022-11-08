#include "sadpch.h"

#include "Bound.h"

/* Set bound position to specified transform position 
*  Assumes that the bound is a cube shape, and the scale gives 
*  the user what it needs to know about width, height, and length values.
* 
*  Rotating the bound hasn't been implemented at the moment.
*/
sad::Bound::Bound(glm::vec3 position, glm::vec3 scale)
	: m_Position(position)
	, m_Scale(scale)
{
	CalculateBound();
}

sad::Bound::Bound(Transform transform)
{
	m_Position = transform.GetPosition();
	m_Scale = transform.GetScale();
	CalculateBound();
}

void sad::Bound::CalculateBound() 
{
	float xMax = m_Position.x + (m_Scale.x / 2);
	float yMax = m_Position.y + (m_Scale.y / 2);
	float zMax = m_Position.z + (m_Scale.z / 2);
	m_Max = glm::vec3(xMax, yMax, zMax);

	float xMin = m_Position.x - (m_Scale.x / 2);
	float yMin = m_Position.y - (m_Scale.y / 2);
	float zMin = m_Position.z - (m_Scale.z / 2);
	m_Min = glm::vec3(xMin, yMin, zMin);
}

bool sad::Bound::Intersects(glm::vec3 otherMax, glm::vec3 otherMin) 
{
	return IntersectionMaxMin(otherMax, otherMin);
}

bool sad::Bound::Intersects(Bound otherBound) {
	glm::vec3 otherBMax = otherBound.GetBoundMax();
	glm::vec3 otherBMin = otherBound.GetBoundMin();

	return IntersectionMaxMin(otherBMax, otherBMin);
}

bool sad::Bound::IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin) {
	return
		m_Min.x <= otherMax.x &&
		m_Max.x >= otherMin.x &&
		m_Min.y <= otherMax.y &&
		m_Max.y >= otherMin.y &&
		m_Min.z <= otherMax.z &&
		m_Max.z >= otherMin.z;
}
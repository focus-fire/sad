#include "sadpch.h"

#include "Bound.h"

/* Set bound position to specified transform position 
*  Assumes that the bound is a cube shape, and the scale gives 
*  the user what it needs to know about width, height, and length values.
* 
*  Rotating the bound hasn't been implemented at the moment.
*/
sad::Bound::Bound(glm::vec3 position, glm::vec3 scale)
{
	m_Position = position;
	m_Scale = scale;
	CalculateBound(m_Position, m_Scale);
}

sad::Bound::Bound(const sad::ecs::TransformComponent transformComponent)
{
	m_Position = transformComponent.m_Transform->GetPosition();
	m_Scale = transformComponent.m_Transform->GetScale();
	CalculateBound(m_Position, m_Scale);
}

sad::Bound::~Bound()
{ }

void sad::Bound::CalculateBound(glm::vec3 position, glm::vec3 scale) {
	float xMax = position.x + (scale.x / 2);
	float yMax = position.y + (scale.y / 2);
	float zMax = position.z + (scale.z / 2);
	m_Max = glm::vec3(xMax, yMax, zMax);

	float xMin = position.x - (scale.x / 2);
	float yMin = position.y - (scale.y / 2);
	float zMin = position.z - (scale.z / 2);
	m_Min = glm::vec3(xMin, yMin, zMin);
}

bool sad::Bound::Intersects(glm::vec3 otherMax, glm::vec3 otherMin) 
{
	return IntersectionMaxMin(otherMax, otherMin);
}

bool sad::Bound::Intersects(sad::ecs::BoundComponent otherBound) {
	glm::vec3 otherBMax = otherBound.m_Bound->GetBoundMax();
	glm::vec3 otherBMin = otherBound.m_Bound->GetBoundMin();
	return IntersectionMaxMin(otherBMax, otherBMin);
}

bool sad::Bound::IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin) {
	return
		m_Min.x <= otherMin.x &&
		m_Max.x >= otherMax.x &&
		m_Min.y <= otherMin.y &&
		m_Max.y >= otherMax.y &&
		m_Min.z <= otherMin.z &&
		m_Max.z >= otherMax.z;
}
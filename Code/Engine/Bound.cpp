#include "sadpch.h"

#include "Bound.h"

sad::Bound::Bound(glm::vec3 center, glm::vec3 size)
	: m_Center()
	, m_Extents()
	, m_Size()
{
	SetCenter(center);
	SetSize(size);
}

sad::Bound::Bound(Transform transform)
	: m_Center()
	, m_Extents()
	, m_Size()
{
	SetCenter(transform.GetPosition());
	SetSize(transform.GetScale());
}

void sad::Bound::SetMinMax(glm::vec3 min, glm::vec3 max)
{
	m_Extents = (max - min) * 0.5f;
	m_Center = min + m_Extents;
}

bool sad::Bound::Intersects(glm::vec3 otherMax, glm::vec3 otherMin) 
{
	return IntersectionMaxMin(otherMax, otherMin);
}

bool sad::Bound::Intersects(Bound otherBound) 
{
	return IntersectionMaxMin(otherBound.GetBoundMax(), otherBound.GetBoundMin());
}

bool sad::Bound::IntersectionMaxMin(glm::vec3 otherMax, glm::vec3 otherMin) {
	return
		GetBoundMin().x <= otherMax.x &&
		GetBoundMax().x >= otherMin.x &&
		GetBoundMin().y <= otherMax.y &&
		GetBoundMax().y >= otherMin.y &&
		GetBoundMin().z <= otherMax.z &&
		GetBoundMax().z >= otherMin.z;
}
#include "sadpch.h"

#include "Bound.h"

sad::Bound::Bound(glm::vec3 center, glm::vec3 size)
	: m_Center()
	, m_Extents()
{
	SetCenter(center);
	SetSize(size);
}

sad::Bound::Bound(glm::vec3 center, glm::vec3 size, glm::vec3 sizeDiff, glm::quat rot)
	: m_Center()
	, m_Extents()
	, m_SizeDifference()
	, m_rotation()
{
	SetSizeDiff(sizeDiff);
	SetCenter(center);
	SetSize(size);
	SetRotation(rot);
}

sad::Bound::Bound(Transform transform)
	: m_Center()
	, m_Extents()
{
	SetCenter(transform.GetPosition());
	SetSize(transform.GetScale());
}

void sad::Bound::SetMinMax(glm::vec3 min, glm::vec3 max)
{
	m_Center = (min + max) * 0.5f;
	m_Extents = (max - min) * 0.5f;
}

void sad::Bound::SetSizeRatio(glm::vec3 scale)
{
	m_SizeDifference = scale / (m_Extents * 2.0f);
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
#include "sadpch.h"

#include "VertexAttribute.h"

#include "Mesh.h"

sad::rad::VertexAttributeContainer::VertexAttributeContainer()
	: m_VertexAttributes()
	, m_Stride(0)
{ }

void sad::rad::VertexAttributeContainer::AddFloatAttribute(unsigned int count)
{
	m_VertexAttributes.push_back({ count, GL_FLOAT, GL_FALSE });
	m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
}

sad::rad::MeshVertexAttributeContainer::MeshVertexAttributeContainer()
	: m_VertexAttributes()
{ }

void sad::rad::MeshVertexAttributeContainer::AddMeshVertexAttribute(unsigned int count, void* offset)
{
	m_VertexAttributes.push_back({ count, GL_FLOAT, GL_FALSE, offset });
}

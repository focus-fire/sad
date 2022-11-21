#include "sadpch.h"

#include "VertexAttribute.h"

#include "Mesh.h"

sad::rad::VertexAttributeContainer::VertexAttributeContainer()
	: m_Stride(0)
{ }

void sad::rad::VertexAttributeContainer::AddFloatAttribute(unsigned int count)
{
	m_VertexAttributes.push_back({ count, GL_FLOAT, GL_FALSE });
	m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
}

void sad::rad::VertexAttributeContainer::AddMeshVertexAttribute(unsigned int count)
{
	m_VertexAttributes.push_back({ count, GL_FLOAT, GL_FALSE });
	m_Stride += sizeof(MeshVertex);
}

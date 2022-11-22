#include "sadpch.h"

#include "Mesh.h"

#include <glad/glad.h>

sad::rad::Mesh::Mesh(const MeshData& meshData)
	: Vertices(meshData.Vertices)
	, Indices(meshData.Indices)
	, Textures(meshData.Textures)
{
	MSetupMesh();
}

sad::rad::Mesh::~Mesh()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
}

void sad::rad::Mesh::MSetupMesh()
{
	constexpr auto stride { sizeof(MeshVertex) };

	m_VertexArray = new VertexArray();
	m_VertexBuffer = new VertexBuffer(&Vertices[0], Vertices.size() * stride);

	MeshVertexAttributeContainer m_VertexAttributes = rad::MeshVertexAttributeContainer();
	m_VertexAttributes.AddMeshVertexAttribute(3, (void*) 0); // Position - vec3
	m_VertexAttributes.AddMeshVertexAttribute(3, (void*) offsetof(MeshVertex, Normal)); // Normal - vec3
	m_VertexAttributes.AddMeshVertexAttribute(2, (void*) offsetof(MeshVertex, TexCoords)); // TexCoord - vec2
	m_VertexArray->AddBufferWithMeshAttributes(*m_VertexBuffer, m_VertexAttributes, stride);

	m_IndexBuffer = new IndexBuffer(&Indices[0], Indices.size());

	m_VertexArray->Unbind();
}
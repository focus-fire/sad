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

void sad::rad::Mesh::Draw(sad::rad::ShaderResource& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + i));

		std::string number;
		std::string name = Textures[i].Type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetUniform1i(("material" + name + number).c_str(), i);
		GL_CALL(glBindTexture(GL_TEXTURE_2D, Textures[i].Id));
	}

	GL_CALL(glActiveTexture(GL_TEXTURE0));

	//GL_CALL(glBindVertexArray(m_Vao));
	GL_CALL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));
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
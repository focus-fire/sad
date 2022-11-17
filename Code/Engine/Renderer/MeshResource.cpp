#include "sadpch.h"

#include "MeshResource.h"

#include <glad/glad.h>

sad::rad::MeshResource::MeshResource(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->Vertices = vertices;
	this->Indices = indices;
	this->Textures = textures;

	sad::rad::MeshResource::MSetupMesh();
}

void sad::rad::MeshResource::Draw(sad::rad::ShaderResource& shader)
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

		shader.SetInt(("material" + name + number).c_str(), i);
		GL_CALL(glBindTexture(GL_TEXTURE_2D, Textures[i].Id));
	}

	GL_CALL(glActiveTexture(GL_TEXTURE0));

	GL_CALL(glBindVertexArray(m_Vao));
	GL_CALL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));
}

void sad::rad::MeshResource::MSetupMesh()
{
	// create buffers/arrays
	GL_CALL(glGenVertexArrays(1, &m_Vao));
	GL_CALL(glGenBuffers(1, &m_Vbo));
	GL_CALL(glGenBuffers(1, &m_Ebo));

	GL_CALL(glBindVertexArray(m_Vao));
	// load data into vertex buffers
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW));

	// vertex Positions
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

	GL_CALL(glBindVertexArray(0));
}
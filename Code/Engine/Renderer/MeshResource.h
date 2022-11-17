#pragma once

#include <glm/glm.hpp>

#include <Engine/Renderer/ShaderResource.h>

#include <Engine/Resource.h>

namespace sad::rad
{
	class MeshResource : public Resource
	{
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		struct Texture
		{
			unsigned int Id;
			std::string Type;
			std::string Path;
		};

		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;

		MeshResource(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void Draw(sad::rad::ShaderResource& shader);

	private:
		unsigned int m_Vao;
		unsigned int m_Vbo;
		unsigned int m_Ebo;

		void MSetupMesh();
	};
}
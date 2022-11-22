#pragma once

#include <glm/glm.hpp>

#include "ShaderResource.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexAttribute.h"

namespace sad::rad
{
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct MeshTexture
	{
		unsigned int Id;
		std::string Type;
		std::string Path;
	};

	struct MeshColor
	{
		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		glm::vec4 Emissive;
	};

	class Mesh
	{
	public:
		struct MeshData
		{
			std::vector<MeshVertex> Vertices;
			std::vector<unsigned int> Indices;
			std::vector<MeshTexture> Textures;
			MeshColor Colors;
		};

		Mesh(const MeshData& meshData);
		~Mesh();

		VertexArray* GetVertexArray() { return m_VertexArray; }
		IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }

	public:
		std::vector<MeshVertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<MeshTexture> Textures;
		MeshColor Colors;

	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;

		void MSetupMesh();
	};
}
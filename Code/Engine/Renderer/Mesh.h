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

	class Mesh
	{
	public:
		struct MeshData
		{
			std::vector<MeshVertex> Vertices;
			std::vector<unsigned int> Indices;
			std::vector<MeshTexture> Textures;
		};

		Mesh(const MeshData& meshData);
		~Mesh();

		void Draw(sad::rad::ShaderResource& shader);

		VertexArray* GetVertexArray() { return m_VertexArray; }
		IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }

	public:
		std::vector<MeshVertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<MeshTexture> Textures;

	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		VertexAttributeContainer* m_VertexAttributes;

		void MSetupMesh();
	};
}
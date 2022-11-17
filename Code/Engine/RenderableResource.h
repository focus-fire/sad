#pragma once

#include <entt/entt.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Engine/Renderer/MeshResource.h>
#include <Engine/Renderer/ShaderResource.h>

#include "Resource.h"

namespace sad
{
	class RenderableResource final : public Resource
	{
	public:
		struct Geometry
		{
			const float* Vertices;
			const unsigned int VertexSize;

			const unsigned int* Indices;
			const unsigned int IndexCount;

			Geometry()
				: Vertices(nullptr)
				, VertexSize(0)
				, Indices(nullptr)
				, IndexCount(0)
			{ }

			explicit Geometry(const float* vertices, const unsigned int vertexSize, const unsigned int* indices, const unsigned int indexCount)
				: Vertices(vertices)
				, VertexSize(vertexSize)
				, Indices(indices)
				, IndexCount(indexCount)
			{ }
		};

		std::vector<sad::rad::MeshResource::Texture> LoadedTextures;

		RenderableResource();
		~RenderableResource();

		explicit RenderableResource(const Resource::ResourceData& resourceData, const Geometry& geometry);

		// TODO: Remove this constructor when 3D models can be parsed - the AddResource for RenderableResource will have to change as well.
		explicit RenderableResource(const Resource::ResourceData& resourceData)
			: Resource(resourceData)
		{ }

		explicit RenderableResource(char* path);

		void Draw(sad::rad::ShaderResource& shader);
		const Geometry GetGeometry() const { return m_Geometry; }

	private:
		Geometry m_Geometry;
		std::vector<sad::rad::MeshResource> m_Meshes;
		std::string m_Directory;

		void MLoadModel(std::string path);
		void MProcessNode(aiNode* node, const aiScene* scene);
		sad::rad::MeshResource MProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<sad::rad::MeshResource::Texture> MLoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int MRetrieveTextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	};
}

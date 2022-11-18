#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Renderer/MeshResource.h>
#include <Engine/Renderer/ShaderResource.h>

namespace sad
{
	class Model
	{
	public:
		std::vector<sad::rad::MeshResource::Texture> LoadedTextures;

		Model();
		~Model();

		explicit Model(char* path);

		void Draw(sad::rad::ShaderResource& shader);

		std::vector<sad::rad::MeshResource> GetMeshes() { return m_Meshes; }

	private:
		std::vector<sad::rad::MeshResource> m_Meshes;
		std::string m_Directory;

		void MLoadModel(std::string path);
		void MProcessNode(aiNode* node, const aiScene* scene);
		sad::rad::MeshResource MProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<sad::rad::MeshResource::Texture> MLoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int MRetrieveTextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	};
}

#pragma once

#include <memory>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/ShaderResource.h>

namespace sad
{
	class Model
	{
	public:
		Model() = default;
		~Model() = default;

		explicit Model(const std::string& modelFilePath);

		std::vector<core::Pointer<rad::Mesh>> GetMeshes() { return m_Meshes; }
		rad::ShaderResource* GetShader() { return m_Shader; }

	public:
		std::vector<rad::MeshTexture> LoadedTextures;

	private:
		void MLoadModel(const std::string& modelFilePath);

		void MProcessNode(aiNode* node, const aiScene* scene);
		rad::Mesh::MeshData MProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<rad::MeshTexture> MLoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int MRetrieveTextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	private:
		std::string m_ModelDirectory;
		std::vector<core::Pointer<rad::Mesh>> m_Meshes;
		rad::ShaderResource* m_Shader = nullptr;
	};
}

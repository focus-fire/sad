#pragma once

#include <memory>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <Engine/Bound.h>
#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/ShaderResource.h>

namespace sad
{
	class RenderableModel
	{
	public:
		RenderableModel() = default;
		~RenderableModel() = default;

		explicit RenderableModel(const std::string& modelFilePath);

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
		aiMatrix4x4 m_ModelMatrix;
		aiAABB m_Bound;

		std::string m_ModelDirectory;
		std::vector<core::Pointer<rad::Mesh>> m_Meshes;
		rad::ShaderResource* m_Shader = nullptr;

	public:
		inline aiAABB GetBound() { return m_Bound; }

		inline glm::mat4 GetModelMatrix()
		{
			glm::mat4 result;
			result[0] = glm::vec4(m_ModelMatrix[0][0], m_ModelMatrix[1][0], m_ModelMatrix[2][0], m_ModelMatrix[3][0]);
			result[1] = glm::vec4(m_ModelMatrix[0][1], m_ModelMatrix[1][1], m_ModelMatrix[2][1], m_ModelMatrix[3][1]);
			result[2] = glm::vec4(m_ModelMatrix[0][2], m_ModelMatrix[1][2], m_ModelMatrix[2][2], m_ModelMatrix[3][2]);
			result[3] = glm::vec4(m_ModelMatrix[0][3], m_ModelMatrix[1][3], m_ModelMatrix[2][3], m_ModelMatrix[3][3]);
			return result;
		}
	};
}

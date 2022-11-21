#include "sadpch.h"

#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "ResourceManager.h"

sad::Model::Model(const std::string& modelFilePath)
{
	m_Meshes.reserve(4);
	m_ModelDirectory = modelFilePath.substr(0, modelFilePath.find_last_of('\\'));

	m_Shader = ResourceManager::GetResource<rad::ShaderResource>("Model.glsl");

	MLoadModel(modelFilePath);
}

void sad::Model::MLoadModel(const std::string& modelFilePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		core::Log(ELogType::Error, "[Model] ERROR::ASSIMP:: {}", importer.GetErrorString());
		return;
	}

	MProcessNode(scene->mRootNode, scene);
}

void sad::Model::MProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all of the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		rad::Mesh::MeshData meshData = MProcessMesh(mesh, scene);
		m_Meshes.push_back(core::CreatePointer<rad::Mesh>(meshData));
	}

	// Perform the same process for each of the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		MProcessNode(node->mChildren[i], scene);
	}
}

sad::rad::Mesh::MeshData sad::Model::MProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<rad::MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<rad::MeshTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		rad::MeshVertex vertex;

		// Process mesh's vertices
		glm::vec3 vector; 

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals 
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// TexCoords 
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texVector;

			texVector.x = mesh->mTextureCoords[0][i].x;
			texVector.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texVector;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	} 

	// Retrieve vertex indices for each mesh's face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Diffuse maps
	std::vector<rad::MeshTexture> diffuseMaps = MLoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// Specular maps
	std::vector<rad::MeshTexture> specularMaps = MLoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// Normal maps
	std::vector<rad::MeshTexture> normalMaps = MLoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// Height maps
	std::vector<rad::MeshTexture> heightMaps = MLoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return rad::Mesh::MeshData(vertices, indices, textures);
}

std::vector<sad::rad::MeshTexture> sad::Model::MLoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<sad::rad::MeshTexture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texName;
		mat->GetTexture(type, i, &texName);

		bool skipTex = false;

		for (unsigned int j = 0; j < LoadedTextures.size(); j++)
		{
			if (std::strcmp(LoadedTextures[j].Path.data(), texName.C_Str()) == 0)
			{
				textures.push_back(LoadedTextures[j]);
				skipTex = true;
				break;
			}
		}

		if (!skipTex)
		{
			sad::rad::MeshTexture texture;
			texture.Id = MRetrieveTextureFromFile(texName.C_Str(), m_ModelDirectory);
			texture.Type = typeName;
			texture.Path = texName.C_Str();
			textures.push_back(texture);
			LoadedTextures.push_back(texture);
		}
	}

	return textures;
}

unsigned int sad::Model::MRetrieveTextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;

	core::Log(ELogType::Info, "[Model] ----------- Retrieving Texture from file: {}", fileName);

	unsigned int textureId;

	GL_CALL(glGenTextures(1, &textureId));

	int width, height, numComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &numComponents, 0);

	if (data)
	{
		GLenum format;

		switch (numComponents)
		{
		case 1:
			format = GL_RED;
		case 3:
			format = GL_RGB;
		case 4:
			format = GL_RGBA;
		}

		GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(data);
	}
	else
	{
		core::Log(ELogType::Error, "[Model] Texture failed to load at path: {}", path);
		stbi_image_free(data);
	}

	return textureId;
}
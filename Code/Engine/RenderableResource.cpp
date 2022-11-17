#include "sadpch.h"

#include "RenderableResource.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "glad/glad.h"

#include "ECS/Registry.h"

sad::RenderableResource::RenderableResource()
	: Resource(Resource::Null())
	, m_Geometry()
{ }

sad::RenderableResource::~RenderableResource()
{ }

sad::RenderableResource::RenderableResource(const Resource::ResourceData& resourceData, const Geometry& geometry)
	: Resource(resourceData)
	, m_Geometry(geometry)
{ }

sad::RenderableResource::RenderableResource(char* path)
{
	MLoadModel(path);
}

void sad::RenderableResource::Draw(sad::rad::ShaderResource& shader)
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
		m_Meshes[i].Draw(shader);
}

void sad::RenderableResource::MLoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		core::Log(ELogType::Error, "[RenderableResource] ERROR::ASSIMP:: {}", importer.GetErrorString());
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('/'));
	MProcessNode(scene->mRootNode, scene);
}

void sad::RenderableResource::MProcessNode(aiNode* node, const aiScene* scene)
{
	// process all of the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(MProcessMesh(mesh, scene));
	}
	// perform the same process for each of the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		MProcessNode(node->mChildren[i], scene);
	}
}

sad::rad::MeshResource sad::RenderableResource::MProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<sad::rad::MeshResource::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<sad::rad::MeshResource::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		sad::rad::MeshResource::Vertex vertex;

		// process mesh's vertices
		glm::vec3 vector; // placeholder to convert assimp vector to glm's vec3

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// texcoords
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
	} // finish processing vertices

	// retrieve vertex indices for each mesh's face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// diffuse maps
	std::vector<sad::rad::MeshResource::Texture> diffuseMaps = MLoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// specular maps
	std::vector<sad::rad::MeshResource::Texture> specularMaps = MLoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// normal maps
	std::vector<sad::rad::MeshResource::Texture> normalMaps = MLoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// height maps
	std::vector<sad::rad::MeshResource::Texture> heightMaps = MLoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return sad::rad::MeshResource(vertices, indices, textures);
}

std::vector<sad::rad::MeshResource::Texture> sad::RenderableResource::MLoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<sad::rad::MeshResource::Texture> textures;

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
			sad::rad::MeshResource::Texture texture;
			texture.Id = MRetrieveTextureFromFile(texName.C_Str(), m_Directory);
			texture.Type = typeName;
			texture.Path = texName.C_Str();
			textures.push_back(texture);
			LoadedTextures.push_back(texture);
		}
	}

	return textures;
}

unsigned int sad::RenderableResource::MRetrieveTextureFromFile(const char* path, const std::string& directory, bool gamma)
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
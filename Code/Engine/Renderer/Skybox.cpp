#include "sadpch.h"
#include "Skybox.h"

#include <Engine/ResourceManager.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <map>

sad::rad::Skybox::~Skybox()
{
    GL_CALL(glDeleteTextures(1, &m_RendererId));
}

void sad::rad::Skybox::SetSkybox()
{
	unsigned int textureID;
    GL_CALL(glGenTextures(1, &m_RendererId));
    Bind();
    
    sad::rad::TextureResource* skyboxArray[6] = {
        m_Top = sad::ResourceManager::GetResource<sad::rad::TextureResource>("top.skybox.jpg"),
        m_Bottom = sad::ResourceManager::GetResource<sad::rad::TextureResource>("bottom.skybox.jpg"), 
        m_Right = sad::ResourceManager::GetResource<sad::rad::TextureResource>("right.skybox.jpg"),
        m_Left = sad::ResourceManager::GetResource<sad::rad::TextureResource>("left.skybox.jpg"),
        m_Front = sad::ResourceManager::GetResource<sad::rad::TextureResource>("front.skybox.jpg"),
        m_Back = sad::ResourceManager::GetResource<sad::rad::TextureResource>("back.skybox.jpg")
    };

    int width, height, nrChannels;
    unsigned char* data;
    std::string path;
    for (unsigned int i = 0; i < 6; i++)
    {
        path = skyboxArray[i]->GetResourceAbsolutePath();
        data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            ));
            stbi_image_free(data);
        } else
        {
            core::Log(ELogType::Warn, "Skybox texture at location: '{}' was not properly loaded", path);
            stbi_image_free(data);
        }
    }

    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

void sad::rad::Skybox::Bind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
}

void sad::rad::Skybox::Unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}


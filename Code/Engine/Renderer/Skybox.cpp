#include "sadpch.h"
#include "Skybox.h"
#include "Sample/Cube.h"
#include "RenderBuddy.h"
#include "VertexAttribute.h"

#include <Engine/ResourceManager.h>
#include <Engine/RenderableResource.h>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stb_image.h>
#include <map>

sad::rad::Skybox::Skybox()
    : m_SkyboxGeometry(PrimitiveResource::Geometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount))
{}

sad::rad::Skybox::~Skybox()
{
    GL_CALL(glDeleteTextures(1, &m_RendererId));

    delete m_VertexArray;
    delete m_VertexBuffer;
    delete m_IndexBuffer;
}

void sad::rad::Skybox::SetSkybox()
{
	unsigned int textureID;
    GL_CALL(glGenTextures(1, &m_RendererId));
    Bind();
    m_Top = sad::ResourceManager::GetResource<sad::rad::TextureResource>("Skybox_Top.skybox.jpg");
    m_Middle = sad::ResourceManager::GetResource<sad::rad::TextureResource>("Skybox_Middle.skybox.jpg");
    m_Bottom = sad::ResourceManager::GetResource<sad::rad::TextureResource>("Skybox_Bottom.skybox.jpg");

    sad::rad::TextureResource* skyboxArray[6] = {
        m_Middle, m_Middle, m_Top, m_Bottom, m_Middle, m_Middle
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
        } else
        {
            core::Log(ELogType::Warn, "Skybox texture at location: '{}' was not properly loaded", path);
        }

        stbi_image_free(data);
    }

    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    // Create vertex array and buffer  
    m_VertexArray = new rad::VertexArray();
    m_VertexBuffer = new rad::VertexBuffer(m_SkyboxGeometry.Vertices, m_SkyboxGeometry.VertexSize);
    rad::VertexAttributeContainer m_VertexAttributes = rad::VertexAttributeContainer();

    // Stripe the data in 'Vertices' appropriately
    m_VertexAttributes.AddFloatAttribute(3);		// Points, Vec3 
    m_VertexAttributes.AddFloatAttribute(2);		// TextureCoordinates, Vec2
    m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, m_VertexAttributes);

    // Create the index buffer for indices
    m_IndexBuffer = new rad::IndexBuffer(m_SkyboxGeometry.Indices, m_SkyboxGeometry.IndexCount);

    m_SkyboxShader = ResourceManager::GetResource<rad::ShaderResource>("Skybox.glsl");
}

void sad::rad::Skybox::Draw()
{
    GL_CALL(glDepthMask(GL_FALSE));
    GL_CALL(glDepthFunc(GL_LEQUAL));
    GL_CALL(glCullFace(GL_FRONT));

    Bind();
    m_SkyboxShader->Bind();

    glm::mat4 viewMatrix = glm::mat4(glm::mat3(rad::RenderBuddy::GetCameraInstance()->GetViewMatrix()));
    glm::mat4 projectionMatrix = rad::RenderBuddy::GetCameraInstance()->GetProjectionMatrix();
    m_SkyboxShader->SetUniformMatrix4fv("u_ViewMatrix", glm::value_ptr(viewMatrix));
    m_SkyboxShader->SetUniformMatrix4fv("u_ProjectionMatrix", glm::value_ptr(projectionMatrix));
    rad::RenderBuddy::DrawIndexed(m_VertexArray, m_IndexBuffer);

    m_SkyboxShader->Unbind();
    Unbind();

    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glDepthFunc(GL_LESS));
    GL_CALL(glDepthMask(GL_TRUE));
}

void sad::rad::Skybox::Bind() const
{
    //GL_CALL(glBindVertexArray(m_RendererId));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
}

void sad::rad::Skybox::Unbind() const
{
    //GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}


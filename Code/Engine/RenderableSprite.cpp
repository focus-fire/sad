#include "sadpch.h"

#include "RenderableSprite.h"

#include <Engine/ResourceManager.h>

#include "Renderer/VertexAttribute.h"

sad::RenderableSprite::RenderableSprite(const std::string& spriteFilePath)
{ 
	// TODO: Cleanup! This construction is really hacky
	m_Texture = ResourceManager::GetResource<rad::TextureResource>(spriteFilePath);

	float vertices[] = 
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	m_Shader = ResourceManager::GetResource<rad::ShaderResource>("Sprite.glsl");

	m_VertexArray = new rad::VertexArray();
	m_VertexBuffer = new rad::VertexBuffer(vertices, sizeof(vertices));
	m_VertexArray->Bind();

	rad::VertexAttributeContainer vertexAttributeContainer = rad::VertexAttributeContainer();
	vertexAttributeContainer.AddFloatAttribute(4);
	m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, vertexAttributeContainer);

	m_VertexArray->Unbind();
}

sad::RenderableSprite::~RenderableSprite()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
}

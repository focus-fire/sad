#include "sadpch.h"

#include "RenderableObject.h"

#include <filesystem>

#include <Engine/ResourceManager.h>

sad::RenderableObject::RenderableObject(RenderableResource* resource)
	: m_RenderableResource(resource)
{
	const RenderableResource::Geometry& geometry = m_RenderableResource->GetGeometry();

	// Create vertex array and buffer  
	m_VertexArray = new sad::rad::VertexArray();
	m_VertexBuffer = new sad::rad::VertexBuffer(geometry.Vertices, geometry.VertexSize); 
	m_VertexAttributes = new sad::rad::VertexAttributeContainer();

	// Stripe the data in 'Vertices' appropriately
	m_VertexAttributes->AddFloatAttribute(3);		// Points, Vec3 
	m_VertexAttributes->AddFloatAttribute(2);		// TextureCoordinates, Vec2
	m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, *m_VertexAttributes);

	// Create the index buffer for indices
	m_IndexBuffer = new sad::rad::IndexBuffer(geometry.Indices, geometry.IndexCount);

	// All RenderableObjects start with this 'Material' by default
	m_Shader = ResourceManager::GetResource<rad::ShaderResource>("Default.glsl");
	m_Shader->Bind();

	// TODO: REMOVE AFTER DEMO
	float r1 = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	float r2 = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	float r3 = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	UniformColor = glm::vec4(r1, r2, r3, 1.0f);

	m_Texture = ResourceManager::GetResource<rad::TextureResource>("Default.png");
	m_Texture->Bind(1);
	m_Shader->SetUniform1i("u_Texture", 1);
}

sad::RenderableObject::~RenderableObject()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_VertexAttributes;
	delete m_IndexBuffer;
}
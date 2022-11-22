#include "sadpch.h"

#include "RenderablePrimitive.h"

#include <filesystem>

#include <Engine/ResourceManager.h>

sad::RenderablePrimitive::RenderablePrimitive(PrimitiveResource* resource)
	: m_PrimitiveResource(resource)
{
	const PrimitiveResource::Geometry& geometry = m_PrimitiveResource->GetGeometry();

	// Create vertex array and buffer  
	m_VertexArray = new rad::VertexArray();
	m_VertexBuffer = new rad::VertexBuffer(geometry.Vertices, geometry.VertexSize); 
	m_VertexAttributes = new rad::VertexAttributeContainer();

	// Stripe the data in 'Vertices' appropriately
	m_VertexAttributes->AddFloatAttribute(3);		// Points, Vec3 
	m_VertexAttributes->AddFloatAttribute(2);		// TextureCoordinates, Vec2
	m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, *m_VertexAttributes);

	// Create the index buffer for indices
	m_IndexBuffer = new rad::IndexBuffer(geometry.Indices, geometry.IndexCount);

	// All RenderablePRimitives start with this 'Material' by default
	m_Shader = ResourceManager::GetResource<rad::ShaderResource>("Default.glsl");
	m_Shader->Bind();

	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	m_Texture = ResourceManager::GetResource<rad::TextureResource>("Default.png");
	m_Texture->Bind(1);
	m_Shader->SetUniform1i("u_Texture", 1);
}

sad::RenderablePrimitive::~RenderablePrimitive()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_VertexAttributes;
	delete m_IndexBuffer;
}
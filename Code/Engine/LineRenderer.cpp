#include "sadpch.h"

#include "LineRenderer.h"

#include <Engine/ResourceManager.h>

sad::LineRenderer::LineRenderer(glm::vec3 from, glm::vec3 to, glm::vec3 color)
	: m_VertexCount(2)
{
	float points[12];
	
	points[0] = from.x;
	points[1] = from.y;
	points[2] = from.z;
	points[3] = color.r;
	points[4] = color.g;
	points[5] = color.b;

	points[6] = to.x;
	points[7] = to.y;
	points[8] = to.z;
	points[9] = color.r;
	points[10] = color.g;
	points[11] = color.b;

	m_Shader = ResourceManager::GetResource<rad::ShaderResource>("Line.glsl");

	m_VertexArray = new rad::VertexArray();
	m_VertexArray->Bind();

	m_VertexBuffer = new rad::VertexBuffer(points, sizeof(points));
	m_VertexBuffer->Bind();
	
	m_VertexAttributeContainer = new rad::VertexAttributeContainer();
	m_VertexAttributeContainer->AddFloatAttribute(3);
	m_VertexAttributeContainer->AddFloatAttribute(3);

	m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, *m_VertexAttributeContainer);
}

sad::LineRenderer::~LineRenderer()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_VertexAttributeContainer;
}

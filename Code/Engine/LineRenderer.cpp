#include "sadpch.h"

#include "LineRenderer.h"

#include <Engine/ResourceManager.h>

sad::LineRenderer::LineRenderer(glm::vec3 from, glm::vec3 to, glm::vec3 color)
	: VertexCount(2)
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

	Shader = ResourceManager::GetResource<rad::ShaderResource>("Line.glsl");

	VertexArray = new rad::VertexArray();
	VertexArray->Bind();

	VertexBuffer = new rad::VertexBuffer(points, sizeof(points));
	VertexBuffer->Bind();
	
	VertexAttributeContainer = new rad::VertexAttributeContainer();
	VertexAttributeContainer->AddFloatAttribute(3);
	VertexAttributeContainer->AddFloatAttribute(3);

	VertexArray->AddBufferWithAttributes(*VertexBuffer, *VertexAttributeContainer);
}

sad::LineRenderer::~LineRenderer()
{
	delete VertexArray;
	delete VertexBuffer;
	delete VertexAttributeContainer;
}

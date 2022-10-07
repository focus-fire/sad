#include "sadpch.h"

#include "Renderer.h"

void sad::rad::Renderer::Clear(float r, float g, float b, float a) const
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_CALL(glClearColor(r * a, g * a, b * a, a));
}

void sad::rad::Renderer::Draw(VertexArray* vertexArray, IndexBuffer* indexBuffer, Shader* shader) const
{
	shader->Bind();
	vertexArray->Bind();
	indexBuffer->Bind();

	GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}

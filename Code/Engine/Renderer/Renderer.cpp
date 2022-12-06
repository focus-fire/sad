#include "sadpch.h"

#include "Renderer.h"

#include <Engine/Application.h>

sad::rad::Renderer::~Renderer()
{
	delete m_FrameBuffer;
	delete m_FrameBufferTexture;
	delete m_RenderBuffer;
}

void sad::rad::Renderer::Start()
{
	const sad::Window* mainWindow = sad::Application::s_MainWindow;
	
	// Initialize GL settings
	GL_CALL(glViewport(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight()));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glCullFace(GL_BACK));
	GL_CALL(glFrontFace(GL_CCW));

	// Create framebuffer 
	m_FrameBuffer = new sad::rad::FrameBuffer(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_FrameBuffer->Bind();

	// Create empty texture and bind it to the framebuffer
	m_FrameBufferTexture = new sad::rad::TextureResource(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_FrameBufferTexture->Bind(0);		// FrameBuffer texture slot should always be 0
	m_FrameBufferTexture->AttachToFramebuffer();

	// Create renderbuffer and bind it to the framebuffer
	m_RenderBuffer = new sad::rad::RenderBuffer(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_RenderBuffer->AttachToFrameBuffer();

	m_FrameBuffer->Unbind();
}

void sad::rad::Renderer::Clear() const
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void sad::rad::Renderer::ClearColor(const glm::vec4& color) const
{
	Clear();
	GL_CALL(glClearColor(color.r * color.a, color.g * color.a, color.b * color.a, color.a));
}

void sad::rad::Renderer::DrawIndexed(VertexArray* vertexArray, IndexBuffer* indexBuffer) const
{
	vertexArray->Bind();
	indexBuffer->Bind();

	GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}

void sad::rad::Renderer::DrawMesh(rad::Mesh* mesh) const
{
	mesh->GetVertexArray()->Bind();
	mesh->GetIndexBuffer()->Bind();

	GL_CALL(glDrawElements(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));

}

void sad::rad::Renderer::DrawLines(VertexArray* vertexArray, unsigned int vertexCount) const
{
	vertexArray->Bind();

	GL_CALL(glDrawArrays(GL_LINES, 0, vertexCount));
}

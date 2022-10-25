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

	// Create framebuffer 
	m_FrameBuffer = new sad::rad::FrameBuffer(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_FrameBuffer->Bind();

	// Create empty texture and bind it to the framebuffer
	m_FrameBufferTexture = new sad::rad::Texture(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_FrameBufferTexture->Bind(0);		// FrameBuffer texture slot should always be 0
	m_FrameBufferTexture->AttachToFramebuffer();

	// Create renderbuffer and bind it to the framebuffer
	m_RenderBuffer = new sad::rad::RenderBuffer(mainWindow->GetWidth(), mainWindow->GetHeight());
	m_RenderBuffer->AttachToFrameBuffer();

	m_FrameBuffer->Unbind();
}

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

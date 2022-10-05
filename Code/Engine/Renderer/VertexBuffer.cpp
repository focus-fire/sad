#include "sadpch.h"

#include "VertexBuffer.h"

#include <glad/glad.h>

#include "Texture.h"

sad::rad::VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GL_CALL(glGenBuffers(1, &m_RendererId));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

sad::rad::VertexBuffer::~VertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &m_RendererId));
}

void sad::rad::VertexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void sad::rad::VertexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

sad::rad::IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: m_IndexCount(count)
{
	GL_CALL(glGenBuffers(1, &m_RendererId));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

sad::rad::IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &m_RendererId));
}

void sad::rad::IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void sad::rad::IndexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

sad::rad::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	GL_CALL(glGenFramebuffers(1, &m_RendererId));
}

sad::rad::FrameBuffer::~FrameBuffer()
{
	GL_CALL(glDeleteFramebuffers(1, &m_RendererId));
}

void sad::rad::FrameBuffer::Bind() const
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));
}

void sad::rad::FrameBuffer::Unbind() const
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

sad::rad::RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
{
	GL_CALL(glGenRenderbuffers(1, &m_RendererId));

	Bind();
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	Unbind();
}

sad::rad::RenderBuffer::~RenderBuffer()
{
	GL_CALL(glDeleteRenderbuffers(1, &m_RendererId));
}

void sad::rad::RenderBuffer::AttachToFrameBuffer()
{
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererId));
}

void sad::rad::RenderBuffer::Bind() const
{
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererId));
}

void sad::rad::RenderBuffer::Unbind() const
{
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

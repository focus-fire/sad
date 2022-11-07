#include "sadpch.h"

#include "RenderBuffer.h"

#include <glad/glad.h>

sad::rad::RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
{
	GL_CALL(glGenRenderbuffers(1, &m_RendererId));

	Bind();

	// Create render buffer for specified width and height
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
	SAD_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Renderbuffer failed to attach to a corresponding framebuffer");
}

void sad::rad::RenderBuffer::Bind() const
{
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererId));
}

void sad::rad::RenderBuffer::Unbind() const
{
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

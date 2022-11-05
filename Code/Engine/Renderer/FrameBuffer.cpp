#include "sadpch.h"

#include "FrameBuffer.h"

#include <glad/glad.h>

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

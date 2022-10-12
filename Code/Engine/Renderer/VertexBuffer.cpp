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


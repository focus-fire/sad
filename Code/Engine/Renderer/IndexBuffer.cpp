#include "sadpch.h"

#include "IndexBuffer.h"

#include <glad/glad.h>

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

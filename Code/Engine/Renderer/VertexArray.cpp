#include "sadpch.h"

#include "VertexArray.h"

#include <glad/glad.h>

sad::rad::VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &m_RendererId));
}

sad::rad::VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererId));
}

void sad::rad::VertexArray::AddBufferWithAttributes(const VertexBuffer& vertexBuffer, const VertexAttributeContainer& vertexAttribContainer)
{
	Bind();
	vertexBuffer.Bind();

	// Get set of vertex attributes from container
	const std::vector<VertexAttribute>& attributes = vertexAttribContainer.GetVertexAttributes();
	unsigned int stripedOffset = 0;

	// Iterate through them
	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		const VertexAttribute attribute = attributes[i];

		// Set the current vertex attribute
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, attribute.Count, attribute.Type, attribute.Normalized, vertexAttribContainer.GetStride(), INT_TO_VOIDP(stripedOffset)));
		
		// Increment offset for next attribute to stripe properly
		stripedOffset += attribute.Count * VertexAttribute::GetSizeOfType(attribute.Type);
	}
}

void sad::rad::VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(m_RendererId));
}

void sad::rad::VertexArray::Unbind() const
{
	GL_CALL(glBindVertexArray(0));
}

#pragma once

#include "VertexBuffer.h"
#include "VertexAttribute.h"

namespace sad::rad
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBufferWithAttributes(const VertexBuffer& vertexBuffer, const VertexAttributeContainer& vertexAttribContainer, bool isMesh = false);

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererId;
	};
}

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

		/**
		 * @brief 
		 * @param vertexBuffer 
		 * @param vertexAttribContainer 
		*/
		void AddBufferWithAttributes(const VertexBuffer& vertexBuffer, const VertexAttributeContainer& vertexAttribContainer);

		/**
		 * @brief 
		 * @param vertexBuffer 
		 * @param vertexAttribContainer 
		 * @param meshStride 
		*/
		void AddBufferWithMeshAttributes(const VertexBuffer& vertexBuffer, const MeshVertexAttributeContainer& vertexAttribContainer, unsigned int meshStride);

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererId;
	};
}

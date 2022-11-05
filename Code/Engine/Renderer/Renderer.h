#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include "Shader.h"

namespace sad::rad
{
	class Renderer
	{
	public:
		~Renderer();

		void Start();

		void BindFrameBuffer() const { return m_FrameBuffer->Bind(); }
		void UnbindFrameBuffer() const { return m_FrameBuffer->Unbind(); }
		unsigned int GetFrameBufferTexture() const { return m_FrameBufferTexture->GetTextureId(); }

		void Clear() const;
		void ClearColor(const glm::vec4& color) const;

		/**
		 * @brief Draws geometry bound by an index buffer
		 * @param vertexArray VAO wrapper for the geometry in GL
		 * @param indexBuffer IBO wrapper for the geometry in GL
		*/
		void DrawIndexed(VertexArray* vertexArray, IndexBuffer* ib) const;

		/**
		 * @brief Draws lines bound to the passed index array
		 * @param vertexArray VAO wrapper for the line geometry in GL
		 * @param vertexCount Number of vertices required for the bound geometry to render
		*/
		void DrawLines(VertexArray* vertexArray, unsigned int vertexCount) const;

	private:
		FrameBuffer* m_FrameBuffer;
		Texture* m_FrameBufferTexture;
		RenderBuffer* m_RenderBuffer;
	};
}

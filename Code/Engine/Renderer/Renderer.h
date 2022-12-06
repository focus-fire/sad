#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "FrameBuffer.h"
#include "TextureResource.h"
#include "RenderBuffer.h"
#include "ShaderResource.h"

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
		 * @brief Draws a mesh bound by an index buffer
		 * @param mesh Mesh wrapper for the geometry of a RenderableModel
		*/
		void DrawMesh(rad::Mesh* mesh) const;

		/**
		 * @brief Draws lines bound to the passed index array
		 * @param vertexArray VAO wrapper for the line geometry in GL
		 * @param vertexCount Number of vertices required for the bound geometry to render
		*/
		void DrawLines(VertexArray* vertexArray, unsigned int vertexCount) const;

		/**
		 * @brief Draws triangles bound to the passed vertex array
		 * @param vertexArray VAO wrapper for the triangles in GL
		 * @param triangleCount Number of triangles to render
		 */
		void DrawTriangles(VertexArray* vertexArray, unsigned int triangleCount) const;

	private:
		FrameBuffer* m_FrameBuffer;
		TextureResource* m_FrameBufferTexture;
		RenderBuffer* m_RenderBuffer;
	};
}

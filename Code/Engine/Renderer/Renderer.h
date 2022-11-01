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

		void BindFrameBuffer() { return m_FrameBuffer->Bind(); }
		void UnbindFrameBuffer() { return m_FrameBuffer->Unbind(); }
		unsigned int GetFrameBufferTexture() { return m_FrameBufferTexture->GetTextureId(); }

		void Clear() const;
		void ClearColor(const glm::vec4& color) const;

		void DrawIndexed(VertexArray* vertexArray, IndexBuffer* ib) const;
		void DrawLines(VertexArray* vertexArray, unsigned int vertexCount) const;

	private:
		FrameBuffer* m_FrameBuffer;
		Texture* m_FrameBufferTexture;
		RenderBuffer* m_RenderBuffer;
	};
}

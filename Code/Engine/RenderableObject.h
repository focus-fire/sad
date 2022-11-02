#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

#include "RenderableResource.h"

namespace sad
{
	/**
	 * @brief Container for geometric data used by OpenGL to render an object.
	 *		  Each RenderableObject must be created with a corresponding RenderableResource.
	*/
	class RenderableObject
	{
	public:
		explicit RenderableObject(RenderableResource* resource);
		~RenderableObject();

		RenderableResource* GetRenderableResource() { return m_RenderableResource; }

		sad::rad::VertexArray* GetVertexArray() { return m_VertexArray; }
		sad::rad::VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
		sad::rad::IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
		sad::rad::Texture* GetTexture() { return m_Texture; }
		sad::rad::Shader* GetShader() { return m_Shader; }

	private:
		RenderableResource* m_RenderableResource;

		sad::rad::VertexArray* m_VertexArray;
		sad::rad::VertexBuffer* m_VertexBuffer;
		sad::rad::VertexAttributeContainer* m_VertexAttributes;
		sad::rad::IndexBuffer* m_IndexBuffer;
		sad::rad::Texture* m_Texture;
		sad::rad::Shader* m_Shader;
	};
}

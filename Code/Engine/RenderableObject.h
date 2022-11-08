#pragma once

#include <glm/glm.hpp>

#include <Engine/ResourceManager.h>

#include "RenderableResource.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/TextureResource.h"
#include "Renderer/ShaderResource.h"

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

		rad::VertexArray* GetVertexArray() { return m_VertexArray; }
		rad::VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
		rad::IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
		rad::TextureResource* GetTexture() { return m_Texture; }
		rad::ShaderResource* GetShader() { return m_Shader; }

		// TODO: Remove after demo
		glm::vec4 UniformColor;

	private:
		RenderableResource* m_RenderableResource;

		rad::VertexArray* m_VertexArray;
		rad::VertexBuffer* m_VertexBuffer;
		rad::VertexAttributeContainer* m_VertexAttributes;
		rad::IndexBuffer* m_IndexBuffer;
		rad::TextureResource* m_Texture;
		rad::ShaderResource* m_Shader;
	};
}

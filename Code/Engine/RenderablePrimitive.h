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
	 * @brief Container for basic shape/geometric data used by OpenGL to render an object.
	 *		  Each RenderablePrimitive must be created with a corresponding PrimitiveResource.
	*/
	class RenderablePrimitive
	{
	public:
		explicit RenderablePrimitive(PrimitiveResource* resource);
		~RenderablePrimitive();

		PrimitiveResource* GetPrimitiveResource() { return m_PrimitiveResource; }

		rad::VertexArray* GetVertexArray() { return m_VertexArray; }
		rad::VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
		rad::IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
		rad::TextureResource* GetTexture() const { return m_Texture; }
		rad::ShaderResource* GetShader() const { return m_Shader; }

	private:
		PrimitiveResource* m_PrimitiveResource;

		rad::VertexArray* m_VertexArray;
		rad::VertexBuffer* m_VertexBuffer;
		rad::VertexAttributeContainer* m_VertexAttributes;
		rad::IndexBuffer* m_IndexBuffer;
		rad::TextureResource* m_Texture;
		rad::ShaderResource* m_Shader;
	};
}

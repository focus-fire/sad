#pragma once

#include <string>

#include "Renderer/ShaderResource.h"
#include "Renderer/TextureResource.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

namespace sad
{
	class RenderableSprite
	{
	public:
		explicit RenderableSprite(const std::string& spriteFilePath);
		~RenderableSprite();

		rad::ShaderResource* GetShader() const { return m_Shader; }
		rad::TextureResource* GetTexture() const { return m_Texture; }
		rad::VertexArray* GetVertexArray() const { return m_VertexArray; }
	
	private:
		rad::ShaderResource* m_Shader;
		rad::TextureResource* m_Texture;

		rad::VertexBuffer* m_VertexBuffer;
		rad::VertexArray* m_VertexArray;
	};
}

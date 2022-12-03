#pragma once

#include "TextureResource.h"
#include "IGLBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderResource.h"

#include <Engine/RenderableResource.h>

namespace sad::rad
{
	class Skybox final : public IGLBuffer
	{
	public:
		Skybox();
		void SetSkybox();
		~Skybox() override;
		void Bind() const override;
		void Unbind() const override;
		void Draw();

	private:
		TextureResource* m_Top = nullptr;
		TextureResource* m_Bottom = nullptr; 
		TextureResource* m_Middle = nullptr;

		VertexArray* m_VertexArray = nullptr;
		VertexBuffer* m_VertexBuffer = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
		ShaderResource* m_SkyboxShader = nullptr;
		PrimitiveResource::Geometry m_SkyboxGeometry;
	};
}




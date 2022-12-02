#pragma once

#include "TextureResource.h"
#include "IGLBuffer.h"
namespace sad::rad
{
	class Skybox final : public IGLBuffer
	{
	public:
		void SetSkybox();
		~Skybox() override;
		void Bind() const override;
		void Unbind() const override;

	private:
		sad::rad::TextureResource* m_Top;
		sad::rad::TextureResource* m_Bottom; 
		sad::rad::TextureResource* m_Right;
		sad::rad::TextureResource* m_Left;
		sad::rad::TextureResource* m_Front;
		sad::rad::TextureResource* m_Back;
	};
}




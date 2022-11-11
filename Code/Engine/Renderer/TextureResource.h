#pragma once

#include <string>

#include <Engine/Resource.h>

namespace sad::rad
{
	class TextureResource final : public Resource 
	{
	public:
		/**
		 * @brief Allocates texture memory and sets default texture parameters for an image 
		 * @param filePath Path to the target texture resource
		*/
		explicit TextureResource(const Resource::ResourceData& resourceData, const std::string& filePath);

		/**
		 * @brief Allocates memory for an empty texture without an image 
		 * @param width Width of the target texture image
		 * @param height Height of the target texture image
		*/
		explicit TextureResource(int width, int height);

		TextureResource();
		~TextureResource();

		void AttachToFramebuffer();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		unsigned int GetTextureId() const { return m_RendererId; }

	private:
		unsigned int m_RendererId;
		unsigned char* m_LocalBuffer;

		int m_Width;
		int m_Height;
		int m_BitsPerPixel;
	};
}

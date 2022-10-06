#pragma once

#include <string>

namespace sad::rad
{
	class TextureBuffer
	{
	public:
		TextureBuffer(const std::string& filePath);
		TextureBuffer(int width, int height);
		~TextureBuffer();

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

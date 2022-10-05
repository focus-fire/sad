#pragma once

#include <string>

namespace sad::rad
{
	class Texture
	{
	public:
		Texture(const std::string& filePath);
		Texture(int width, int height);
		~Texture();

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

#include "sadpch.h"

#include "TextureResource.h"

#include <stb_image.h>
#include <glad/glad.h>

sad::rad::TextureResource::TextureResource(const IResource::ResourceData& resourceData, const std::string& filePath)
	: IResource(resourceData)
{
	// Flip texture upside-down as OpenGL expects pixels to start at bottom-left	
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

	GL_CALL(glGenTextures(1, &m_RendererId));
	SAD_ASSERT(m_RendererId, "Failed to generate texture in GL");

	Bind();

	// Set default texture parameters
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Create 2D texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	core::Log(ELogType::Trace, "[TextureResource] Texture created with ID #{}", m_RendererId);

	Unbind();

	// Free unneeded image data 
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

sad::rad::TextureResource::TextureResource(int width, int height)
	: IResource({ "FrameBuffer.fake", "FrameBuffer.fake" })
	, m_Width(width)
	, m_Height(height)
{
	GL_CALL(glGenTextures(1, &m_RendererId));
	SAD_ASSERT(m_RendererId, "Failed to generate texture in GL");

	Bind();

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// ALlocate memory for empty texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	core::Log(ELogType::Trace, "[TextureResource] Texture created with ID #{}", m_RendererId);

	Unbind();
}

sad::rad::TextureResource::~TextureResource()
{
	GL_CALL(glDeleteTextures(1, &m_RendererId));
}

void sad::rad::TextureResource::AttachToFramebuffer()
{
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererId, 0));
}

void sad::rad::TextureResource::Bind(unsigned int slot /* = 0 */) const
{
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void sad::rad::TextureResource::Unbind() const
{
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

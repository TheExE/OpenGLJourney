#include "Texture.h"
#include "vendor/stb_image/stb_image.h"
#include "Renderer.h"
#include <GL/glew.h>

Texture::Texture(const std::string& filePath)
{
	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(filePath.c_str(),
		&m_width, &m_height,
		&m_bitsPerPixel, 4);

	// Create and start working with this texture
	GLCall(glGenTextures(1, &m_rendererId));
	Bind();

	// Set texture parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Send texture data to GPU
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, m_localBuffer));

	// Stop working with this texuture
	Unbind();

	// Free the texture memory from CPU side
	if(m_localBuffer != nullptr)
	{
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture()
{
	// Free texture data
	GLCall(glDeleteTextures(1, &m_rendererId));
}

void Texture::Bind(unsigned int slot) const
{
	// One texture may have different representations stored in separate slot
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));

	// Start working with this texuture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}

void Texture::Unbind() const
{
	// Stop working with this texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

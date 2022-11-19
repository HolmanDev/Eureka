#include "pch.h"
#include "Texture.h"

#include "Renderer.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path, const TextureSpecification& specifications)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Specification(specifications), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // stbi_load writes to all the variables

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, specifications.ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, specifications.ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, specifications.WrapMode)); // Tiling is also an option
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, specifications.WrapMode));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::Texture(int width, int height)
	: m_RendererID(0), m_LocalBuffer(nullptr),
	m_Width(width), m_Height(height), m_BPP(0), m_Specification({GL_LINEAR, GL_CLAMP_TO_EDGE })
{

}

Texture::Texture()
	: m_RendererID(0), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0), m_Specification({ GL_LINEAR, GL_CLAMP_TO_EDGE })
{
	
}

Texture::Texture(const Texture& obj)
	: m_RendererID(obj.m_RendererID), m_LocalBuffer(nullptr),
	m_Width(obj.m_Width), m_Height(obj.m_Height), m_BPP(0), m_FilePath(obj.GetFilePath()), 
	m_Specification(obj.GetSpecification())
{
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetID(unsigned int id)
{
	m_RendererID = id;
}

void Texture::SetSpecification(const TextureSpecification& specification)
{
	m_Specification = specification;
}

#pragma once

#include "engine/glCore.h"
#include "TextureSpecification.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	TextureSpecification m_Specification;
public:
	Texture(const std::string& path, const TextureSpecification& specifications);
	Texture(int width, int height);
	Texture();
	Texture(const Texture& obj);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	const inline std::string GetFilePath() const { return m_FilePath; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	const inline unsigned int GetID() const { return m_RendererID; }
	TextureSpecification GetSpecification() const { return m_Specification; }

	void SetID(unsigned int id);
	void SetSpecification(const TextureSpecification& specification);
};
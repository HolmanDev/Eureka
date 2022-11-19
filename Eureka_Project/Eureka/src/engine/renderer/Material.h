#pragma once

#include "pch.h"

#include "Shader.h"
#include "Texture.h"

class Material
{
private:
	std::shared_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;
	std::unique_ptr<Texture> m_Depth;
	glm::vec4 m_Color;
public:
	Material(std::shared_ptr<Shader> shader);
	~Material();

	void UpdateMaterial();
	void Bind();
	void Unbind();

	inline const std::shared_ptr<Shader> GetShader() const { return m_Shader; }
	inline const Texture& GetTexture() const { return *m_Texture; }

	void SetShader(std::shared_ptr<Shader> shader);
	void SetTexture(Texture* texture);
	void SetDepth(Texture& depth);
	void SetColor(glm::vec4 color);
};
#include "pch.h"
#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader)
	: m_Shader(shader), m_Texture(nullptr), m_Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
}

Material::~Material()
{
}

void Material::UpdateMaterial()
{
	SetTexture(m_Texture.get());
	SetColor(m_Color);
}

void Material::Bind()
{
	m_Shader->Bind();
}

void Material::Unbind()
{
	m_Shader->Unbind();
}

void Material::SetShader(std::shared_ptr<Shader> shader)
{
	m_Shader = shader;
	//m_Shader->SetUniform1f("u_UseTexture", 0.0f); //! Is this needed?
}

void Material::SetTexture(Texture* texture)
{
	m_Shader->Bind();
	if (texture == nullptr)
	{
		m_Shader->SetUniform1f("u_UseTexture", 0.0f);
	} 
	else
	{
		m_Texture = std::make_unique<Texture>(texture->GetFilePath(), texture->GetSpecification() );
		m_Texture->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform1f("u_UseTexture", 1.0f);
	}
	m_Shader->Unbind();
}

void Material::SetDepth(Texture& depth)
{
	m_Shader->Bind();
	m_Depth = std::make_unique<Texture>(depth);
	m_Depth->Bind(1);
	m_Shader->SetUniform1i("u_DepthTexture", 1);
	m_Shader->Unbind();
}

void Material::SetColor(glm::vec4 color)
{
	m_Shader->Bind();
	m_Color = color;
	m_Shader->SetUniform4f("u_Color", color.r, color.g, color.b, color.a);
	m_Shader->Unbind();
}

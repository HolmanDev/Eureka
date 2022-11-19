#include "pch.h"
#include "UIElement.h"

#include "engine/glCore.h"

UIElement::UIElement()
	: m_Position(glm::vec2(0)), m_Texture(nullptr), m_Framebuffer(nullptr)
{
	m_Framebuffer = std::make_unique<Framebuffer>();
}

UIElement::~UIElement()
{
}

void UIElement::Invalidate()
{
	m_Framebuffer->Bind();
	m_Material->UpdateMaterial();
	m_Framebuffer->AddColorAttachment(m_Material->GetTexture().GetID());
	m_Framebuffer->Unbind();
}

void UIElement::SetID(unsigned int id)
{
	m_ID = id;
}

void UIElement::SetName(const std::string& name)
{
	m_Name = name;
}

void UIElement::SetPosition(glm::vec2 position)
{
	m_Position = position;
}

void UIElement::SetSize(glm::vec2 size)
{
	m_Size = size;
}

void UIElement::SetTexture(std::shared_ptr<Texture> texture)
{
	m_Texture = texture;
}

void UIElement::SetMaterial(std::shared_ptr<Material> material)
{
	m_Material = material;
}

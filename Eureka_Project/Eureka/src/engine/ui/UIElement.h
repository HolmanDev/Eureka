#pragma once

#include "engine/renderer/Framebuffer.h"
#include "engine/renderer/Texture.h"
#include "engine/renderer/Material.h"

class UIElement
{
private:
	unsigned int m_ID;
	std::string m_Name;
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Material> m_Material;
	std::unique_ptr<Framebuffer> m_Framebuffer;
public:
	UIElement();
	~UIElement();

	void Invalidate();

	unsigned int GetID() { return m_ID; }
	std::string GetName() { return m_Name; }
	glm::vec2 GetPosition() const { return m_Position; }
	glm::vec2 GetSize() const { return m_Size; }
	std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
	const std::shared_ptr<Material> GetMaterial() const { return m_Material; }
	Framebuffer& GetFramebuffer() const { return *m_Framebuffer; }

	void SetID(unsigned int id);
	void SetName(const std::string& name);
	void SetPosition(glm::vec2 position);
	void SetSize(glm::vec2 size);
	void SetTexture(std::shared_ptr<Texture> texture);
	void SetMaterial(std::shared_ptr<Material> material);
};
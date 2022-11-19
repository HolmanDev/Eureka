#include "pch.h"
#include "GameObject.h"

#include "engine/events/EventManager.h"
#include "engine/concepts/AttributeManager.h"

GameObject::GameObject()
	: m_ID(0), m_Name("New GameObject"), m_Transform(std::make_unique<Transform>())
{
	
}

GameObject::GameObject(std::string name)
	: m_ID(0), m_Name(name), m_Transform(std::make_unique<Transform>())
{
	
}

GameObject::~GameObject()
{
	
}

std::vector<std::shared_ptr<Attribute>>& GameObject::GetAllAttributes() const
{
	return AttributeManager::GetAllAttributes(m_ID);
}

void GameObject::SetID(unsigned int id)
{
	m_ID = id;
}

void GameObject::SetName(std::string name)
{
	m_Name = name;
}

std::shared_ptr<Attribute> GameObject::AddAttribute(std::shared_ptr<Attribute> attribute)
{
	return AttributeManager::AddAttribute(m_ID, attribute);
}

void GameObject::RemoveAttribute(std::shared_ptr<Attribute> attribute)
{
	AttributeManager::RemoveAttribute(m_ID, attribute);
}

//! Think about this one
void GameObject::Destroy()
{
	AttributeManager::RemoveGameObject(m_ID);
}

std::shared_ptr<Attribute> GameObject::GetAttribute(const std::type_info& typeInfo)
{
	return AttributeManager::GetAttribute(m_ID, typeInfo);
}

std::vector<std::shared_ptr<Attribute>> GameObject::GetAttributes(const std::type_info& typeInfo)
{
	return AttributeManager::GetAttributes(m_ID, typeInfo);
}

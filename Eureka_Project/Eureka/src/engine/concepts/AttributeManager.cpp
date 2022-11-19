#include "pch.h"
#include "AttributeManager.h"

#include "engine/events/EventManager.h"

std::map<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<Attribute>>> AttributeManager::s_AttributeMap;

void AttributeManager::Close()
{
	s_AttributeMap.clear();
}

void AttributeManager::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	s_AttributeMap[gameObject] = std::vector<std::shared_ptr<Attribute>>();
}

void AttributeManager::RemoveGameObject(unsigned int id)
{
	s_AttributeMap.erase(FindGameObjectFromID(id));
}

void AttributeManager::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	s_AttributeMap.erase(gameObject);
}

std::vector<std::shared_ptr<Attribute>>& AttributeManager::GetAllAttributes(unsigned int id)
{
	return FindAttributesFromID(id);
}

std::vector<std::shared_ptr<Attribute>>& AttributeManager::GetAllAttributes(std::shared_ptr<GameObject> gameObject)
{
	return s_AttributeMap[gameObject];
}

std::shared_ptr<Attribute> AttributeManager::GetAttribute(unsigned int id, const std::type_info& typeInfo)
{
	return GetAttribute(FindGameObjectFromID(id), typeInfo);
}

std::shared_ptr<Attribute> AttributeManager::GetAttribute(std::shared_ptr<GameObject> gameObject, const std::type_info& typeInfo)
{
	std::vector<std::shared_ptr<Attribute>>& attributes = GetAllAttributes(gameObject);
	for (auto&& attribute : attributes)
	{
		if (attribute->Identify() == typeInfo.name()) {
			return attribute;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<Attribute>> AttributeManager::GetAttributes(unsigned int id, const std::type_info& typeInfo)
{
	return GetAttributes(FindGameObjectFromID(id), typeInfo);
}

std::vector<std::shared_ptr<Attribute>> AttributeManager::GetAttributes(std::shared_ptr<GameObject> gameObject, const std::type_info& typeInfo)
{
	std::vector<std::shared_ptr<Attribute>>& attributes = GetAllAttributes(gameObject);
	std::vector<std::shared_ptr<Attribute>> attributesSpec;
	for (auto&& attribute : attributes)
	{
		if (attribute->Identify() == typeInfo.name()) {
			attributesSpec.push_back(attribute);
		}
	}
	return attributesSpec;
}

std::shared_ptr<Attribute> AttributeManager::AddAttribute(unsigned int id, std::shared_ptr<Attribute> attribute)
{
	attribute->SetGameObject(FindGameObjectFromID(id));
	std::vector<std::shared_ptr<Attribute>>& attributes = FindAttributesFromID(id);
	attributes.push_back(attribute);
	return attribute;
}

std::shared_ptr<Attribute> AttributeManager::AddAttribute(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Attribute> attribute)
{
	attribute->SetGameObject(gameObject);
	std::vector<std::shared_ptr<Attribute>>& attributes = gameObject->GetAllAttributes();
	attributes.push_back(attribute);
	return attribute;
}

void AttributeManager::RemoveAttribute(unsigned int id, std::shared_ptr<Attribute> attribute)
{
	RemoveAttribute(FindGameObjectFromID(id), attribute);
}

void AttributeManager::RemoveAttribute(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Attribute> attribute_ptr)
{
	std::vector<std::shared_ptr<Attribute>>& attributes = GetAllAttributes(gameObject);
	EventManager::RemoveSubscriber(std::dynamic_pointer_cast<EventSubscriber>(attribute_ptr));
	//! Replace everything below with erase_if
	std::vector<int> dump;
	int i = 0;
	for (auto&& attribute : attributes)
	{
		if (attribute == attribute_ptr) {
			dump.push_back(i);
		}
		i++;
	}
	for (int index : dump)
	{
		attributes.erase(attributes.begin() + index);
	}
}

// -- PRIVATE ---
std::shared_ptr<GameObject> AttributeManager::FindGameObjectFromID(unsigned int id)
{
	auto it = std::find_if(s_AttributeMap.begin(), s_AttributeMap.end(),
		[id](std::pair<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<Attribute>>> t) {
		return t.first->GetID() == id;
	});
	return it->first;
}

std::vector<std::shared_ptr<Attribute>>& AttributeManager::FindAttributesFromID(unsigned int id)
{
	auto it = std::find_if(s_AttributeMap.begin(), s_AttributeMap.end(),
		[id](std::pair<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<Attribute>>> t) {
		return t.first->GetID() == id;
	});
	return it->second;
}

#pragma once
#include "Attribute.h"

#include "engine/world/GameObject.h" //! use class GameObject instead?

class AttributeManager
{
private:
	static std::map<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<Attribute>>> s_AttributeMap;
public:
	static void Close();

	static void AddGameObject(std::shared_ptr<GameObject> gameObject);
	static void RemoveGameObject(unsigned int id);
	static void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	static std::vector<std::shared_ptr<Attribute>>& GetAllAttributes(unsigned int id);
	static std::vector<std::shared_ptr<Attribute>>& GetAllAttributes(std::shared_ptr<GameObject> gameObject);
	template<class T> static std::shared_ptr<T> GetAttribute(unsigned int id);
	template<class T> static std::shared_ptr<T> GetAttribute(std::shared_ptr<GameObject> gameObject);
	static std::shared_ptr<Attribute> GetAttribute(unsigned int id, const std::type_info& typeInfo);
	static std::shared_ptr<Attribute> GetAttribute(std::shared_ptr<GameObject> gameObject, const std::type_info& typeInfo);
	template<class T> static std::vector<std::shared_ptr<Attribute>> GetAttributes(unsigned int id);
	template<class T> static std::vector<std::shared_ptr<Attribute>> GetAttributes(std::shared_ptr<GameObject> gameObject);
	static std::vector<std::shared_ptr<Attribute>> GetAttributes(unsigned int id, const std::type_info& typeInfo);
	static std::vector<std::shared_ptr<Attribute>> GetAttributes(std::shared_ptr<GameObject> gameObject, const std::type_info& typeInfo);

	static std::shared_ptr<Attribute> AddAttribute(unsigned int id, std::shared_ptr<Attribute> attribute);
	static std::shared_ptr<Attribute> AddAttribute(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Attribute> attribute);
	static void RemoveAttribute(unsigned int id, std::shared_ptr<Attribute> attribute);
	static void RemoveAttribute(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Attribute> attribute_ptr);
private:
	static std::shared_ptr<GameObject> FindGameObjectFromID(unsigned int id);
	static std::vector<std::shared_ptr<Attribute>>& FindAttributesFromID(unsigned int id);
};

template<class T>
inline std::shared_ptr<T> AttributeManager::GetAttribute(unsigned int id)
{
	return std::dynamic_pointer_cast<T>(GetAttribute(id, typeid(T)));
}

template<class T>
inline std::shared_ptr<T> AttributeManager::GetAttribute(std::shared_ptr<GameObject> gameObject)
{
	return std::dynamic_pointer_cast<T>(GetAttribute(gameObject, typeid(T)));
}

template<class T>
inline std::vector<std::shared_ptr<Attribute>> AttributeManager::GetAttributes(unsigned int id)
{
	std::vector<std::shared_ptr<Attribute>> attributes = GetAttributes(id, typeid(T));
	for (auto& attribute : attributes)
	{
		attribute = std::dynamic_pointer_cast<T>(attribute);
	}
	return attributes;
}

template<class T>
inline std::vector<std::shared_ptr<Attribute>> AttributeManager::GetAttributes(std::shared_ptr<GameObject> gameObject)
{
	std::vector<std::shared_ptr<Attribute>> attributes = GetAttributes(gameObject, typeid(T));
	std::vector<std::shared_ptr<T>> attributesSpec;
	for (auto& attribute : attributes)
	{
		attributesSpec.push_back(std::dynamic_pointer_cast<T>(attribute));
	}
	return attributesSpec;
}

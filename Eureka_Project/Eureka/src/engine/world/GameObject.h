#pragma once

#include "engine/attributes/Mesh.h"
#include "engine/attributes/Transform.h"

class GameObject
{
private:
	unsigned int m_ID;
	std::string m_Name;
	std::unique_ptr<Transform> m_Transform;
public:
	GameObject();
	GameObject(std::string name);
	~GameObject();

	inline unsigned int GetID() { return m_ID; }
	inline std::string GetName() { return m_Name; }
	inline Transform& GetTransform() const { return *m_Transform; }
	inline const glm::mat4 GetModelMatrix() const { return m_Transform->GetMatrix(); };
	std::vector<std::shared_ptr<Attribute>>& GetAllAttributes() const;

	void SetID(unsigned int id);
	void SetName(std::string name);

	template<class T> std::shared_ptr<T> GetAttribute();
	template<class T> std::vector<std::shared_ptr<T>> GetAttributes();
	std::shared_ptr<Attribute> AddAttribute(std::shared_ptr<Attribute> attribute);
	void RemoveAttribute(std::shared_ptr<Attribute> attribute);

	void Destroy();
private:
	std::shared_ptr<Attribute> GetAttribute(const std::type_info& typeInfo);
	std::vector<std::shared_ptr<Attribute>> GetAttributes(const std::type_info& typeInfo);
};

template<class T>
std::shared_ptr<T> GameObject::GetAttribute()
{
	return std::dynamic_pointer_cast<T>(GetAttribute(typeid(T)));
}

template<class T>
std::vector<std::shared_ptr<T>> GameObject::GetAttributes()
{
	std::vector<std::shared_ptr<Attribute>> attributes = GetAttributes(typeid(T));
	std::vector<std::shared_ptr<T>> attributesSpec;
	for (auto& attribute : attributes)
	{
		attributesSpec.push_back(std::dynamic_pointer_cast<T>(attribute));
	}
	return attributesSpec;
}

#pragma once
#include "pch.h"

class GameObject;

// Only one gameobject can hold an attribute, but it can be moved between gameobjects
class Attribute
{
private:
	std::weak_ptr<GameObject> m_GameObject;
public:
	virtual ~Attribute();

	virtual std::string Identify() = 0;

	std::weak_ptr<GameObject> GetGameObject() const { return m_GameObject; }
	void SetGameObject(std::shared_ptr<GameObject> gameObject);
};
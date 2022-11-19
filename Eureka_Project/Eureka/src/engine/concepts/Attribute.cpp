#include "pch.h"
#include "Attribute.h"

#include "engine/world/GameObject.h"

Attribute::~Attribute()
{
}

void Attribute::SetGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_GameObject = gameObject;
}

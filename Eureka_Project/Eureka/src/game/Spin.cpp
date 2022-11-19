#include "pch.h"
#include "Spin.h"

#include "engine/core/Time.h"
#include "engine/world/GameObject.h"
#include "engine/events/EventManager.h"

Spin::Spin()
{
}

Spin::~Spin()
{
}

std::string Spin::Identify()
{
	return typeid(*this).name();
}

void Spin::Start()
{
}

void Spin::Update()
{
	if (auto gameObject = GetGameObject().lock()) {
		float angle = Time::GetTimestep() * 2.0f * glm::pi<float>();
		Transform& transform = gameObject->GetTransform();
		transform.RotateEuler(angle, glm::vec3(0, 1, 0));
	}
}

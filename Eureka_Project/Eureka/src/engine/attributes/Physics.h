#pragma once

#include "engine/events/EventSubscriber.h"
#include "engine/concepts/Attribute.h"
#include "Transform.h"

class Physics : public Attribute, public EventSubscriber
{
private:
	Transform& m_Transform;
	float m_Mass;
	glm::vec3 m_LastPosition;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
public:
	Physics(Transform& transform);
	~Physics();

	std::string Identify() override;

	void Update() override;

	const Transform& GetTransform() const { return m_Transform; }
	glm::vec3 GetVelocity() const { return m_Velocity; }
	void SetVelocity(glm::vec3 velocity);
	void AddVelocity(glm::vec3 deltaVelocity);
	glm::vec3 GetAcceleration() const { return m_Acceleration; }
	void SetAcceleration(glm::vec3 acceleration);
	void AddAcceleration(glm::vec3 deltaAcceleration);

	void Force(float force, glm::vec3 direction);
};
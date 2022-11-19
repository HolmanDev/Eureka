#include "pch.h"
#include "Physics.h"

#include "engine/core/Time.h"

Physics::Physics(Transform& transform)
	: m_Transform(transform), m_Mass(1.0f), m_Velocity(glm::vec3(0.0f)), m_Acceleration(glm::vec3(0.0f))
{
	m_LastPosition = transform.GetPosition();
}

Physics::~Physics()
{
}

std::string Physics::Identify()
{
	return typeid(*this).name();
}

void Physics::Update()
{
	float timestep = Time::GetTimestep();
	glm::vec3 newPosition = m_Transform.GetPosition() + m_Velocity * timestep;
	m_Transform.SetPosition(newPosition);
	m_Velocity = (newPosition - m_LastPosition) / timestep;
	m_LastPosition = newPosition;
	//! Add drag
}

void Physics::SetVelocity(glm::vec3 velocity)
{
	m_Velocity = velocity;
}

void Physics::AddVelocity(glm::vec3 deltaVelocity)
{
	m_Velocity = m_Velocity + deltaVelocity;
}

void Physics::SetAcceleration(glm::vec3 acceleration)
{
	m_Acceleration = acceleration;
}

void Physics::AddAcceleration(glm::vec3 deltaAcceleration)
{
	m_Acceleration = m_Acceleration + deltaAcceleration;
}

void Physics::Force(float force, glm::vec3 direction)
{
	direction = glm::normalize(direction);
	glm::vec3 acceleration = direction * (force / m_Mass);
	float timestep = Time::GetTimestep();
	glm::vec3 position = m_Transform.GetPosition();
	m_Transform.SetPosition(position + acceleration * (timestep * timestep * 0.5f));
}

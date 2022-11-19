#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Transform::~Transform()
{
}

const glm::mat4 Transform::GetMatrix()
{
	glm::mat4 matrix(1.0f);
	matrix = glm::scale(matrix, m_Scale);
	glm::vec3 worldPos = glm::vec3(m_Position.x / m_Scale.x, m_Position.y / m_Scale.y, m_Position.z / m_Scale.z);
	matrix = glm::translate(matrix, worldPos) * glm::toMat4(m_Rotation);
	return matrix;
}

void Transform::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

void Transform::SetRotation(glm::quat rotation)
{
	m_Rotation = rotation;
}

void Transform::Rotate(glm::quat rotation)
{
	m_Rotation = glm::normalize(rotation * m_Rotation);
}

void Transform::RotateEuler(float angle, glm::vec3 dir)
{
	glm::vec3 normDir = glm::normalize(dir);
	glm::quat rotation = glm::cos(angle * 0.5f) * glm::quat(1, 0, 0, 0) + glm::sin(angle * 0.5f) * glm::quat(0, normDir.x, normDir.y, normDir.z);
	m_Rotation = glm::normalize(rotation * m_Rotation);
}

void Transform::RotateEuler2(float angle, glm::vec3 dir)
{
	glm::vec3 normDir = glm::normalize(dir);
	glm::quat rotation = glm::cos(angle * 0.5f) * glm::quat(1, 0, 0, 0) + glm::sin(angle * 0.5f) * glm::quat(0, normDir.x, normDir.y, normDir.z);
	m_Rotation = glm::normalize(m_Rotation * rotation );
}

void Transform::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

const glm::vec3 Transform::GetRight() const
{
	return glm::rotate(m_Rotation, glm::vec3(1, 0, 0));
}

const glm::vec3 Transform::WorldRightToLocal()
{
	return glm::vec3(0);
}

const glm::vec3 Transform::GetUp() const
{
	return glm::rotate(m_Rotation, glm::vec3(0, 1, 0));
}

const glm::vec3 Transform::WorldUpToLocal()
{
	return glm::vec3(0);
}

const glm::vec3 Transform::GetForward() const
{
	return glm::rotate(m_Rotation, glm::vec3(0, 0, -1));
}

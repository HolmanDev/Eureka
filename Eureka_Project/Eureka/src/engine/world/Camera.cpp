#include "pch.h"
#include "Camera.h"

#include "engine/world/GameObject.h"
#include "engine/core/Time.h"
#include "engine/core/InputManager.h"
#include "game/GameManager.h"

Camera::Camera(float ratio)
	: m_ProjectionMatrix(glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f)), 
	m_MovementEnabled(true), m_Pitch(0), m_Yaw(0)
{
}

Camera::~Camera()
{
}

std::string Camera::Identify()
{
	return typeid(*this).name();
}

void Camera::Start()
{
	//! Can this be removed?
}

void Camera::Update()
{
	if(m_MovementEnabled && !GameManager::IsPaused())
		LookAround();
}

const glm::mat4 Camera::GetViewMatrix() const
{
	if (auto gameObject = GetGameObject().lock())
	{
		return glm::inverse(gameObject->GetModelMatrix());
	}
	return glm::mat4(1.0);
}

void Camera::EnableMovement(bool state)
{
	m_MovementEnabled = state;
}

void Camera::LookAround()
{
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	Transform& transform = gameObject->GetTransform();
	glm::vec2 mouseDelta = InputManager::GetMouseDelta();
	int tPitch = (int) (m_Pitch / (2.0f * glm::pi<float>()));
	int tYaw = (int) (m_Yaw / (2.0f * glm::pi<float>()));
	m_Pitch += mouseDelta.y * GameManager::GetSensitivity() * Time::GetTimestep();
	m_Pitch -= tPitch * 2.0f * glm::pi<float>();
	m_Yaw += mouseDelta.x * GameManager::GetSensitivity() * Time::GetTimestep();
	m_Yaw -= tYaw * 2.0f * glm::pi<float>();
	transform.SetRotation(glm::normalize(glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f))));
}

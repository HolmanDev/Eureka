#pragma once

#include "engine/concepts/Attribute.h"
#include "engine/events/EventSubscriber.h"
#include "engine/attributes/Transform.h"

class Camera: public Attribute, public EventSubscriber
{
private:
	glm::mat4 m_ProjectionMatrix;
	float m_Pitch;
	float m_Yaw;
	bool m_MovementEnabled;
public:
	Camera(float ratio);
	~Camera();

	std::string Identify() override;

	void Start() override;
	void Update() override;

	const glm::mat4 GetViewMatrix() const;
	const inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	//! These should be based of a list of blockers instead
	void EnableMovement(bool state);
	bool MovementIsEnabled() const { return m_MovementEnabled; }
private:
	void LookAround();
};
#pragma once

#include "engine/concepts/Attribute.h"
#include "engine/events/EventSubscriber.h"
#include "engine/world/Camera.h"
#include "game/inventions/Part.h"
#include "engine/physics/Ray.h"

enum class PlayerState { Default, ConfigChunk };

class Player : public Attribute, public EventSubscriber
{
private:
	std::shared_ptr<Camera> m_Camera;
	std::string m_SelectedPartType;
	bool m_MovementEnabled;
	bool m_Configuring;
	Part* m_ConfiguringPart;
	bool m_Typing;
	int m_SelectedSize;
	PlayerState m_State;
public:
	Player();
	~Player();

	std::string Identify() override;
	void Init();

	void Start() override;
	void Update() override;

	std::shared_ptr<Camera> GetCamera() const { return m_Camera; }
	const int GetSelectedSize() const { return m_SelectedSize; }
	const PlayerState GetState() const { return m_State; }

	void SetCamera(std::shared_ptr<Camera> camera);
	void SetConfiguring(bool configuring, Part* part);
	void SetSelectedSize(int size);
	void SetState(PlayerState state);

	//! These should be based of a list of blockers instead
	void EnableMovement(bool state);
	bool MovementIsEnabled() const { return m_MovementEnabled; }
	bool IsConfiguring() const { return m_Configuring; }
	bool IsTyping() const { return m_Typing; }
	void SetTyping(bool state);
private:
	void TryMove();
	void TryInteract();
	void TryPlacePart(const std::string partType);
	void TryRemovePart();
	void TogglePause();
	void SelectBlockType(const std::string partType);

	RayHit CastCameraRay(std::shared_ptr<Camera> camera);
	std::shared_ptr<GameObject> GetTargetGameObject();
	std::shared_ptr<Part> GetTargetPart(std::shared_ptr<Invention>* invention = nullptr, RayHit* hit = nullptr);
	void AddPart(std::shared_ptr<Invention> invention, std::shared_ptr<Part> part);
	void RemovePart(std::shared_ptr<Invention> invention, std::shared_ptr<Part> part);
	glm::vec3 GetNewPartPosition(glm::vec3 point, std::shared_ptr<Part> targetPart, const Transform& inventionTransform, int size);
};
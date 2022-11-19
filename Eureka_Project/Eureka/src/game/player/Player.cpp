#include "pch.h"
#include "Player.h"

#include "engine/glCore.h"

#include "engine/core/WindowInfo.h"
#include "engine/core/Time.h"
#include "engine/world/GameObject.h"
#include "engine/events/EventManager.h"
#include "engine/core/InputManager.h"
#include "game/GameManager.h"
#include "game/inventions/Invention.h"
#include "engine/world/World.h"
#include "game/inventions/parts/Parts.h"
#include "game/ui/UIManager.h"
#include "game/inventions/InventionManager.h"

Player::Player()
	: m_Camera(nullptr), m_SelectedPartType("structure"), m_MovementEnabled(true), m_Configuring(false), 
	m_ConfiguringPart(nullptr), m_Typing(false), m_SelectedSize(1), m_State(PlayerState::Default)
{
}

Player::~Player()
{
}

std::string Player::Identify()
{
	return typeid(*this).name();
}

void Player::Start()
{

}

void Player::Update()
{
	TryMove();
	TryInteract();
}

void Player::Init()
{
	if (auto gameObject = GetGameObject().lock()) {
		m_Camera = std::make_shared<Camera>((float)WindowInfo::width / (float)WindowInfo::height);
		EventManager::AddSubscriber(m_Camera);
		gameObject->AddAttribute(m_Camera);
	}
}

void Player::EnableMovement(bool state)
{
	m_MovementEnabled = state;
}

void Player::SetCamera(std::shared_ptr<Camera> camera)
{
	m_Camera = camera;
}

void Player::SetConfiguring(bool configuring, Part* part)
{
	m_Configuring = configuring;
	if (m_Configuring)
	{
		m_Camera->EnableMovement(false);
		m_ConfiguringPart = part;
		if (part->GetPartType() == "chunk")
		{
			InputManager::SetCursorMode(GLFW_CURSOR_DISABLED);
			SetState(PlayerState::ConfigChunk);
		}
		else {
			InputManager::SetCursorMode(GLFW_CURSOR_NORMAL);
		}
	} 
	else
	{
		m_Camera->EnableMovement(true);
		m_ConfiguringPart = nullptr;
		SetState(PlayerState::Default);
		InputManager::SetCursorMode(GLFW_CURSOR_DISABLED);
	}
}

void Player::SetTyping(bool state)
{
	m_Typing = state;
}

void Player::SetSelectedSize(int size)
{
	m_SelectedSize = size;
}

void Player::SetState(PlayerState state)
{
	m_State = state;
}

// Handle player movement
void Player::TryMove()
{
	if (GameManager::IsPaused() || !m_MovementEnabled || m_Typing) return;

	//! Unsure if this guard clause works
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	Transform& transform = gameObject->GetTransform();
	float speed = 2.0f;

	if (m_State == PlayerState::Default)
	{
		// Forward
		if (InputManager::KeyPressed(GLFW_KEY_W))
		{
			glm::vec3 dir = transform.GetForward();
			dir.y = 0;
			dir = glm::normalize(dir);
			transform.SetPosition(transform.GetPosition() + dir * speed * Time::GetTimestep());
		}

		// Backward
		if (InputManager::KeyPressed(GLFW_KEY_S))
		{
			glm::vec3 dir = transform.GetForward();
			dir.y = 0;
			dir = glm::normalize(dir);
			transform.SetPosition(transform.GetPosition() - dir * speed * Time::GetTimestep());
		}

		// Right
		if (InputManager::KeyPressed(GLFW_KEY_D))
		{
			transform.SetPosition(transform.GetPosition() + transform.GetRight() * speed * Time::GetTimestep());
		}

		// Left
		if (InputManager::KeyPressed(GLFW_KEY_A))
		{
			transform.SetPosition(transform.GetPosition() - transform.GetRight() * speed * Time::GetTimestep());
		}

		// Up
		if (InputManager::KeyPressed(GLFW_KEY_SPACE))
		{
			transform.SetPosition(transform.GetPosition() + glm::vec3(0, 1, 0) * speed * Time::GetTimestep());
		}

		// Down
		if (InputManager::KeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			transform.SetPosition(transform.GetPosition() - glm::vec3(0, 1, 0) * speed * Time::GetTimestep());
		}
	}
	else if (m_State == PlayerState::ConfigChunk)
	{
		pChunk* chunk = (pChunk*)m_ConfiguringPart;
		Grid3Pos selectedPosition = chunk->GetSelectedPosition();

		// Move selection
		if (InputManager::KeyPressedDown(GLFW_KEY_W))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(0, 0, 1));
		if (InputManager::KeyPressedDown(GLFW_KEY_S))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(0, 0, -1));
		if (InputManager::KeyPressedDown(GLFW_KEY_A))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(-1, 0, 0));
		if (InputManager::KeyPressedDown(GLFW_KEY_D))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(1, 0, 0));
		if (InputManager::KeyPressedDown(GLFW_KEY_E))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(0, 1, 0));
		if (InputManager::KeyPressedDown(GLFW_KEY_Q))
			chunk->SetSelectedPosition(selectedPosition + Grid3Pos(0, -1, 0));
	}
}

// Handle interactions with the environment
void Player::TryInteract()
{
	if (InputManager::KeyPressedDown(GLFW_KEY_TAB))
	{
		TogglePause();
	}

	// Chunk interaction
	if (m_State == PlayerState::ConfigChunk)
	{
		std::shared_ptr<Invention> invention = m_ConfiguringPart->GetInvention();
		pChunk* chunk = (pChunk*)m_ConfiguringPart;
		Grid3Pos selectedPosition = chunk->GetSelectedPosition();

		if (InputManager::KeyPressedDown(GLFW_KEY_SPACE))
		{
			chunk->TryPlacePart(m_SelectedPartType);
		}
	}

	if (GameManager::IsPaused() || m_Configuring || m_Typing) return;

	RayHit hit;
	std::shared_ptr<Invention> invention;
	std::shared_ptr<Part> targetPart = GetTargetPart(&invention, &hit);
	// Interaction
	if (targetPart)
	{
		assert(invention);
		if (m_State == PlayerState::Default)
		{
			if (InputManager::KeyPressedDown(GLFW_MOUSE_BUTTON_1))
			{
				TryRemovePart();
			}

			if (InputManager::KeyPressedDown(GLFW_MOUSE_BUTTON_2))
			{
				if (m_SelectedPartType == "empty")
				{
					//TryInteractWithPart();
					targetPart->Interact(this, invention.get());
				}
				else
				{
					TryPlacePart(m_SelectedPartType);
				}
			}

			if (InputManager::KeyPressedDown(GLFW_KEY_E))
			{
				targetPart->Configure(this, invention.get());
			}
		}
	}

	// Toggle size
	if (InputManager::KeyPressedDown(GLFW_KEY_T))
	{
		if (GetSelectedSize() == 1)
		{
			SetSelectedSize(2);
		}
		else if (GetSelectedSize() == 2)
		{
			SetSelectedSize(1);
		}
	}

	// Block selections
	if (InputManager::KeyPressedDown(GLFW_KEY_Q))
	{
		m_SelectedPartType = "empty";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/HandIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_1))
	{
		m_SelectedPartType = "structure";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/StructureIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_2))
	{
		m_SelectedPartType = "spitter";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/GasIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_3))
	{
		m_SelectedPartType = "wire";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/WireIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_4))
	{
		m_SelectedPartType = "light";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/LightIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_5))
	{
		m_SelectedPartType = "power_source";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/PowerSourceIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_6))
	{
		m_SelectedPartType = "controller";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/ControllerIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE }); //! Update
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}

	if (InputManager::KeyPressedDown(GLFW_KEY_7))
	{
		m_SelectedPartType = "chunk";
		std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
		Texture texture("res/textures/ChunkIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE }); //! Update
		uiElement->GetMaterial()->SetTexture(&texture);
		uiElement->Invalidate();
	}
}

// Place part if possible
void Player::TryPlacePart(const std::string partType)
{
	if (partType == "empty") return;
	RayHit hit = CastCameraRay(m_Camera);
	std::shared_ptr<GameObject> targetGameObject = GetTargetGameObject();
	if (!targetGameObject) return;
	std::shared_ptr<Invention> invention = targetGameObject->GetAttribute<Invention>();
	if (!invention) return;
	std::shared_ptr<Part> targetPart = invention->GetPartWithCollider(hit.collider);
	if (!targetPart) return;

	Transform& inventionTransform = targetGameObject->GetTransform();
	glm::vec3 position = GetNewPartPosition(hit.position, targetPart, inventionTransform, m_SelectedSize);
	std::vector<std::shared_ptr<MeshCollider>> meshColliders = targetGameObject->GetAttributes<MeshCollider>();
	for (std::shared_ptr<MeshCollider> meshCollider : meshColliders)
	{
		bool occupied = glm::distance2(meshCollider->center, position) < 0.25f; // 0.5 * 0.5 = 0.25
		if (occupied) return;
	}
	std::shared_ptr<Part> newPart;
	switch (Part::partTypes.at(partType))
	{
	case 0: newPart = std::make_shared<pStructure>(); break;
	case 1: newPart = std::make_shared<pSpitter>(); break;
	case 2: newPart = std::make_shared<pLight>(); break;
	case 3: newPart = std::make_shared<pWire>(); break;
	case 4: newPart = std::make_shared<pPowerSource>(); break;
	case 5: newPart = std::make_shared<pController>(); break;
	case 6: newPart = std::make_shared<pChunk>(); break;
	default: std::cout << "Error: Specified type doesn't exist" << std::endl; break;
	}
	newPart->SetPosition(position);
	AddPart(invention, newPart);
}

// Remove part if possible
void Player::TryRemovePart()
{
	RayHit hit = CastCameraRay(m_Camera);
	std::shared_ptr<GameObject> targetGameObject = GetTargetGameObject();
	if (!targetGameObject) return;
	std::shared_ptr<Invention> invention = targetGameObject->GetAttribute<Invention>();
	if (!invention) return;
	std::shared_ptr<Part> targetPart = invention->GetPartWithCollider(hit.collider);
	if (!targetPart) return;

	RemovePart(invention, targetPart);
}

void Player::TogglePause()
{
	if (GameManager::IsPaused())
	{
		GameManager::Unpause();
		InputManager::SetCursorMode(GLFW_CURSOR_DISABLED);
	}
	else
	{
		GameManager::Pause();
		InputManager::SetCursorMode(GLFW_CURSOR_NORMAL);
	}
}

void Player::SelectBlockType(const std::string partType)
{
	m_SelectedPartType = partType;
	std::shared_ptr<UIElement> uiElement = GameManager::GetWorld()->GetUIElement("blockTypeUI");
	Texture texture("res/textures/ControllerIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE }); //! Get icon from some config file
	uiElement->GetMaterial()->SetTexture(&texture);
	uiElement->Invalidate();
}

// Cast a ray though the camera
RayHit Player::CastCameraRay(std::shared_ptr<Camera> camera)
{
	auto cameraGameObject = camera->GetGameObject().lock();
	if (!cameraGameObject) return RayHit();

	Transform& cameraTransform = cameraGameObject->GetTransform();
	glm::vec3 direction = cameraTransform.GetForward();
	Ray ray = Ray(cameraTransform.GetPosition(), direction, 100.0f); //! Use camera near and far instead
	return ray.Cast(GameManager::GetWorld());
}

// Return the GameObject that the camera looks at
std::shared_ptr<GameObject> Player::GetTargetGameObject()
{
	RayHit hit = CastCameraRay(m_Camera);
	if (!hit.collided) return nullptr;
	return hit.collider->GetGameObject().lock();
}

// Finds the target part based on its collider
std::shared_ptr<Part> Player::GetTargetPart(std::shared_ptr<Invention>* invention, RayHit* hit)
{
	*hit = CastCameraRay(m_Camera);
	if (!hit->collided) return nullptr;
	if (auto collidedGameObject = hit->collider->GetGameObject().lock())
	{
		//! hmm
		*invention = collidedGameObject->GetAttribute<Invention>();
		if (*invention)
			return (*invention)->GetPartWithCollider(hit->collider);
	}
	return nullptr;
}

void Player::AddPart(std::shared_ptr<Invention> invention, std::shared_ptr<Part> part)
{
	invention->AddPart(part);
	invention->GenerateMesh();
	invention->ApplyMesh();
	invention->GenerateColliders();
	invention->ApplyColliders();
}

void Player::RemovePart(std::shared_ptr<Invention> invention, std::shared_ptr<Part> part)
{
	invention->RemovePart(part);
	invention->GenerateMesh();
	invention->ApplyMesh();
	invention->GenerateColliders();
	invention->ApplyColliders();
}

// This should be relative to vehicle
glm::vec3 Player::GetNewPartPosition(glm::vec3 point, std::shared_ptr<Part> targetPart, const Transform& inventionTransform, int size)
{
	glm::vec3 partPosition = targetPart->GetPosition();
	glm::vec3 inventionPosition = inventionTransform.GetPosition();
	glm::vec3 partRelativePosition = point - inventionPosition - partPosition;
	float right = glm::dot(partRelativePosition, inventionTransform.GetRight());
	float up = glm::dot(partRelativePosition, inventionTransform.GetUp());
	float forward = glm::dot(partRelativePosition, inventionTransform.GetForward());
	float directionAbsValues[3] = { glm::abs(right), glm::abs(up), glm::abs(forward) };
	int directionIndex = std::max_element(&directionAbsValues[0], &directionAbsValues[3]) - &directionAbsValues[0]; //! The 3 is important
	float sizeFactor = 1.0f / size;
	switch (directionIndex)
	{
	case 0: return partPosition + inventionTransform.GetRight() * (right > 0 ? 1.0f : -1.0f) * sizeFactor;		// Right
	case 1: return partPosition + inventionTransform.GetUp() * (up > 0 ? 1.0f : -1.0f) * sizeFactor;			// Up
	case 2: return partPosition + inventionTransform.GetForward() * (forward > 0 ? 1.0f : -1.0f) * sizeFactor;	// Forward
	}
	return glm::vec3(0);
}
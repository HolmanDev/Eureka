#include "pch.h"
#include "pController.h"

#include "engine/core/InputManager.h"
#include "engine/world/GameObject.h"
#include "game/ui/UIManager.h"
#include "game/inventions/Invention.h"

pController::pController() : Part(Part::GenerateSpecification(GetPath())),
	m_IsEngaged(false), m_Player(nullptr), m_KeyCode(-1)
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_XFace = data["xFace"];
	m_YFace = data["yFace"];
	m_ZFace = data["zFace"];
}

pController::~pController()
{

}

std::string pController::GetPath()
{
	return "res/bits/controller.json";
}

float* pController::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i += 2)
	{
		int face = i / 8;
		switch (face)
		{
		case 0:
		case 1: faceNum = m_ZFace; break;
		case 2:
		case 3: faceNum = m_XFace; break;
		default: faceNum = m_YFace; break;
		}
		x = (float)(faceNum % 21);
		y = (float)(faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

//! Make player assignment work through a GUI
void pController::SetPlayer(Player* player)
{
	m_Player = player;
}

void pController::SetKeyCode(unsigned int keyCode)
{
	m_KeyCode = keyCode;
}

void pController::SetEngagedState(bool state)
{
	m_IsEngaged = state;
	if (m_Player)
	{
		m_Player->EnableMovement(!state);
		if (!state)
			m_Player = nullptr;
	}
	else
	{
		LOG_WARNING("Changed state before assigning player.");
	}
	assert(m_IsEngaged == state);
}

void pController::Action(Invention* invention)
{
	VERIFY_ARG(invention);

	SetPlayer(invention->GetPlayer());
	if (m_IsEngaged)
	{
		if (m_Player)
		{
			ControlPlayer(m_Player, invention);
		}
	}
}

void pController::Interact(Player* player, Invention* invention)
{
	Part::Interact(player, invention);
	SetEngagedState(!IsEngaged()); // Toggle engaged state
	invention->GenerateMesh();
	invention->ApplyMesh();
}

// Begin the configuration of the controlller
void pController::Configure(Player* player, Invention* invention)
{
	if (player->IsConfiguring()) return;
	Part::Configure(player, invention);
	UIManager::ControllerConfig(this); // Display UI
}

void pController::StopConfiguring(Player* player, Invention* invention)
{
	Part::StopConfiguring(player, invention);
}

void pController::ControlPlayer(Player* player, Invention* invention)
{
	if (!m_IsEngaged)
	{
		LOG_WARNING("Tried to control player when controller wasn't activated.");
		return;
	}
	VERIFY_ARG(player);
	VERIFY_ARG(invention);

	player->EnableMovement(false);

	// Disengage
	if (InputManager::KeyPressedDown(GLFW_KEY_R))
	{
		SetEngagedState(false);
		SetPoweredState(false);
		UpdateSurrounding(invention);
		invention->GenerateMesh();
		invention->ApplyMesh();
		return;
	}

	// Power ON
	if (InputManager::KeyPressedDown(m_KeyCode))
	{
		SetPoweredState(true);
		UpdateSurrounding(invention);
	}

	// Power OFF
	if (InputManager::KeyPressedUp(m_KeyCode))
	{
		SetPoweredState(false);
		UpdateSurrounding(invention);
	}
}

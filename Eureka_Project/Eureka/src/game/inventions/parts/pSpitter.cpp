#include "pch.h"
#include "pSpitter.h"

#include "engine/core/Time.h"
#include "engine/world/GameObject.h"
#include "engine/attributes/Physics.h"
#include "game/inventions/invention.h"
#include "game/player/Player.h"
#include "game/ui/UIManager.h"

pSpitter::pSpitter() : Part(Part::GenerateSpecification(GetPath()))
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_SideFace = data["sideFace"];
	m_FrontFace = data["frontFace"];
	m_BackFace = data["backFace"];
	m_Direction = Direction::Right;
}

pSpitter::~pSpitter()
{
}

std::string pSpitter::GetPath()
{
	return "res/bits/spitter.json";
}

float* pSpitter::GetUVs()
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
		case 0: faceNum = m_FrontFace; break;
		case 1: faceNum = m_BackFace; break;
		default: faceNum = m_SideFace; break;
		}
		x = (float)(faceNum % 21);
		y = (float)(faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
	//! Change to local directions
}

void pSpitter::SetDirection(Direction direction)
{
	m_Direction = direction;
}

void pSpitter::Spit(Invention* invention)
{
	// Apply up force to invention
	if (auto gameObject = invention->GetGameObject().lock()) {
		glm::vec3 position = gameObject->GetTransform().GetPosition();
		//gameObject->GetTransform().SetPosition(position + glm::vec3(0.0, 0.2, 0.0) * Time::GetTimestep());
		std::shared_ptr<Physics> physics = gameObject->GetAttribute<Physics>();
		if (physics)
		{
			physics->Force(0.1f, DirectionHandler::ToVec3(m_Direction));
		}
	}
}

// Performs an action
void pSpitter::Action(Invention* invention) //! hmm
{
	if (IsPowered())
	{
		Spit(invention);
	}
}

void pSpitter::OnEnvironmentChange(Invention* invention)
{
	UpdatePower(this, invention); // Find all blocks that are directly powered
	invention->GenerateMesh();
	invention->ApplyMesh();
}

void pSpitter::Configure(Player* player, Invention* invention)
{
	if (player->IsConfiguring()) return;
	Part::Configure(player, invention);
	UIManager::SpitterConfig(player, this); // Display UI
}

void pSpitter::StopConfiguring(Player* player, Invention* invention)
{
	Part::StopConfiguring(player, invention);
}

#include "pch.h"
#include "Part.h"

#include "game/inventions/invention.h"
#include "game/inventions/InventionManager.h"
#include "game/player/Player.h"

const std::map<std::string, unsigned int> Part::partTypes {
		{ "structure", 0 },
		{ "spitter", 1 },
		{ "light", 2 },
		{ "wire", 3 },
		{ "power_source", 4 },
		{ "controller", 5 },
		{ "chunk", 6}
};

Part::Part()
	: m_Specification({ "empty", false, false }), m_InventionKey(-1), m_Pos(glm::vec3(0, 0, 0)), m_Color(glm::vec3(1, 1, 1)), 
	m_Configuring(false), m_Size(1)
{
}

Part::Part(const PartSpecification& specification)
	: m_Specification(specification), m_InventionKey(-1), m_Pos(glm::vec3(0, 0, 0)), m_Color(glm::vec3(1, 1, 1)), m_Configuring(false), m_Size(1)
{
}

Part::~Part()
{
}

float* Part::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum = 0;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i+=2)
	{
		faceNum = i / 8;
		x = float(faceNum);
		y = 0;
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i+1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

void Part::OnEnvironmentChange(Invention* invention)
{
}

void Part::UpdateSurrounding(Invention* invention)
{
	std::vector<Part*> connectingParts = GetConnectingParts(invention);
	for (Part* part : connectingParts)
	{
		if (!part) continue;
		part->OnEnvironmentChange(invention);
	}
}

void Part::OnPlace(Invention* invention)
{
	OnEnvironmentChange(invention);
	UpdateSurrounding(invention);
}

void Part::OnRemove(Invention* invention)
{
	UpdateSurrounding(invention);
}

void Part::Interact(Player* player, Invention* invention)
{
	if (!m_Specification.Interactable) return;
}

void Part::Configure(Player* player, Invention* invention)
{
	if (!m_Specification.Configurable) return;
	m_Configuring = true;
	player->SetConfiguring(true, this);
}

void Part::StopConfiguring(Player* player, Invention* invention)
{
	if (!m_Specification.Configurable) return;
	m_Configuring = false;
	player->SetConfiguring(false, nullptr);
	player->GetCamera()->EnableMovement(true);
}

std::shared_ptr<Invention> Part::GetInvention() const
{ 
	return InventionManager::GetInvention(m_InventionKey); 
}

std::vector<Part*> Part::GetConnectingParts(Invention* invention)
{
	Grid3D<Part*>& partSpace = invention->GetPartSpace();
	glm::vec3 position = GetPosition();
	int x = (int)round(position.x);
	int y = (int)round(position.y);
	int z = (int)round(position.z);
	std::vector<Part*> parts;
	if (partSpace.InRange(x + 1, y, z))
		parts.push_back(partSpace.Access(x + 1, y, z));
	if (partSpace.InRange(x - 1, y, z))
		parts.push_back(partSpace.Access(x - 1, y, z));
	if (partSpace.InRange(x, y + 1, z))
		parts.push_back(partSpace.Access(x, y + 1, z));
	if (partSpace.InRange(x, y - 1, z))
		parts.push_back(partSpace.Access(x, y - 1, z));
	if (partSpace.InRange(x, y, z + 1))
		parts.push_back(partSpace.Access(x, y, z + 1));
	if (partSpace.InRange(x, y, z - 1))
		parts.push_back(partSpace.Access(x, y, z - 1));
	return parts;
}

void Part::SetPosition(glm::vec3 position)
{
	m_Pos = position;
}

void Part::SetColor(glm::vec3 color)
{
	m_Color = color;
}

void Part::SetHitBox(std::shared_ptr<Collider> hitbox)
{
	m_HitBox = hitbox;
}

void Part::SetInventionKey(int key)
{
	m_InventionKey = key;
}

void Part::SetSize(int size)
{
	m_Size = size;
}

const PartSpecification Part::GenerateSpecification(const std::string& path)
{
	std::ifstream f(path);
	json data = json::parse(f);
	return PartSpecification{ data["name"], data["interactable"], data["configurable"] };
}


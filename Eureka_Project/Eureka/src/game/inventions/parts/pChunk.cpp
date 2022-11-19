#include "pch.h"
#include "pChunk.h"

#include "game/player/Player.h"
#include "Parts.h"
#include "game/inventions/Invention.h"
#include "game/inventions/InventionManager.h"

class NotImplementedException : public std::logic_error
{
public:
	NotImplementedException() : std::logic_error{ "Function not yet implemented." } {}
};

pChunk::pChunk() : Part(Part::GenerateSpecification(GetPath())),
	m_SelectedPart(nullptr), m_SelectedPosition(Grid3Pos(0, 0, 0))
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_FaceTex = data["face"];
}

pChunk::~pChunk()
{
}

std::string pChunk::GetPath()
{
	return "res/bits/chunk.json";
}

float* pChunk::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum = m_FaceTex;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i += 2)
	{
		x = (float)(faceNum % 21);
		y = (float)(faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

void pChunk::AddPart(Part* part)
{
	std::shared_ptr<Invention> invention = GetInvention();

	m_Parts.push_back(part);
	part->SetSize(2);
	glm::vec3 position = part->GetPosition();
	if (m_PartSpace.InRange(m_SelectedPosition))
	{
		Part*& oldPart = m_PartSpace.Access(m_SelectedPosition);
		if (oldPart)
		{
			if (oldPart->GetPartType() == "empty")
			{
				delete oldPart;
				oldPart = nullptr;
			}
		}
	}
	m_PartSpace.Set(part, m_SelectedPosition);
	part->OnPlace(invention.get());
	std::cout << "Added part" << std::endl;
}

void pChunk::RemovePart(Part* part)
{
	auto end = std::remove_if(m_Parts.begin(), m_Parts.end(), [&](Part* iPart) { return iPart == part; });
	m_Parts.resize(end - m_Parts.begin());
}

void pChunk::TryPlacePart(const std::string partType)
{
	std::shared_ptr<Invention> invention = GetInvention();

	// Perform occupation check

	// Create bit
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
	newPart->SetSize(2.0f);
	glm::vec3 position = GetPosition();
	position.x += m_SelectedPosition.x / 9.0f;
	position.y += m_SelectedPosition.y / 9.0f;
	position.z += m_SelectedPosition.z / 9.0f;
	newPart->SetPosition(position);
	AddPart(newPart.get());
	invention->AddToPartList(newPart);
	invention->GenerateMesh();
	invention->ApplyMesh();
	invention->GenerateColliders();
	invention->ApplyColliders();
}

void pChunk::Clear()
{
	m_Parts.clear();
}

const Part* pChunk::GetPart() const
{
	//return std::find_if(m_Parts.begin(), m_Parts.end(), [&](std::shared_ptr<Part> iPart))
	throw NotImplementedException();
}

void pChunk::SetSelectedPart(Part* part)
{
	m_SelectedPart = part;
}

void pChunk::SetSelectedPosition(Grid3Pos pos)
{
	m_SelectedPosition = pos;
	if (m_PartSpace.InRange(pos))
	{
		m_SelectedPart = m_PartSpace.Access(pos);
	}
	else {
		m_SelectedPart = nullptr;
	}

	std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}

void pChunk::Action(Invention* invention)
{
}

void pChunk::Configure(Player* player, Invention* invention)
{
	if (player->IsConfiguring()) return;
	Part::Configure(player, invention);
}

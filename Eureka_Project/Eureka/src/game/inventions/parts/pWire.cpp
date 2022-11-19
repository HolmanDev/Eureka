#include "pch.h"
#include "pWire.h"

#include "engine/core/Time.h"
#include "engine/world/GameObject.h"
#include "game/inventions/invention.h"
#include "game/inventions/parts/Parts.h"
#include "game/inventions/WireConnection.h"

pWire::pWire() : Part(Part::GenerateSpecification(GetPath())),
	m_WireConnection(nullptr)
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_FaceTex = data["face"];
	m_PoweredFaceTex = data["poweredFace"];
}

pWire::~pWire()
{
}

std::string pWire::GetPath()
{
	return "res/bits/wire.json";
}

float* pWire::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum = m_FaceTex;
	if (m_WireConnection->IsPowered())
		faceNum = m_PoweredFaceTex;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i += 2)
	{
		x = (float) (faceNum % 21);
		y = (float) (faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

void pWire::ConnectToWires(Invention* invention)
{
	Grid3Pos gridPosition = Grid3Pos::FromVec3(GetPosition());
	std::vector<Part*> connectingParts = GetConnectingParts(invention);
	std::vector<WireConnection*> wireConnections;
	bool foundOtherConnections = false;
	for (Part* part : connectingParts)
	{
		if (!part) continue;
		if (part->GetPartType() != "wire") continue;
		pWire* wire = dynamic_cast<pWire*>(part);
		WireConnection* wireConnection = wire->GetWireConnection();
		if (wireConnection)
		{
			if (wireConnection != m_WireConnection)
			{
				foundOtherConnections = true;
			}
			bool wireConnectionAlreadyFound = std::find(wireConnections.begin(), wireConnections.end(), wireConnection) != wireConnections.end();
			if (!wireConnectionAlreadyFound) 
			{
				wireConnections.push_back(wireConnection);
			}
		}
		else
		{
			LOG_WARNING("Could not find wire connection of wire!");
		}
	}

	// Keep the same wire connection group if no other connection groups are found
	if (!foundOtherConnections && m_WireConnection) return;

	// If the wire can't find any other wires whatsoever, it must create its own wire connection group
	if (wireConnections.empty())
	{
		m_WireConnection = new WireConnection();
		m_WireConnection->AddWirePart(this);
		return;
	}

	// If only one connection is found, simply add this wire to that connection group
	if (wireConnections.size() == 1)
	{
		m_WireConnection = wireConnections[0];
		m_WireConnection->AddWirePart(this);
		return;
	}

	// Merge different wire connection groups
	if (wireConnections.size() > 0)
	{
		wireConnections[0]->BuildConnectionsFromWirePart(this, invention);
		m_WireConnection = wireConnections[0];
		for (unsigned int i = 1; i < wireConnections.size(); i++)
		{
			delete wireConnections[i];
		}
	}
}

void pWire::SetWireConnection(WireConnection* wireConnection)
{
	m_WireConnection = wireConnection;
}

void pWire::FindConnections(std::vector<Grid3Pos>& path, Invention* invention)
{
	Grid3Pos gridPosition = Grid3Pos::FromVec3(GetPosition());
	path.push_back(gridPosition);
	std::vector<Part*> connectingParts = GetConnectingParts(invention);
	for (Part* part : connectingParts)
	{
		if (!part) continue;
		if (part->GetPartType() != "wire") continue;
		Grid3Pos partGridPosition = Grid3Pos::FromVec3(part->GetPosition());
		bool isInPath = std::find(path.begin(), path.end(), partGridPosition) != path.end();
		if (isInPath) continue;

		pWire* wire = dynamic_cast<pWire*>(part);
		wire->FindConnections(path, invention);
	}
}

void pWire::SetPoweredState(bool state)
{
}

void pWire::Action(Invention* invention)
{
}

void pWire::OnRemove(Invention* invention)
{
	m_WireConnection->RemoveWirePart(this);
	Part::OnRemove(invention);
}

// An EnvironentChange cannot cause another environment change (without a delay that is)
void pWire::OnEnvironmentChange(Invention* invention)
{
	ConnectToWires(invention);
	m_WireConnection->BuildConnections(invention);
	m_WireConnection->UpdatePoweredState(invention);
	invention->GenerateMesh(); //! Move these calls to end of frame instead
	invention->ApplyMesh();
}

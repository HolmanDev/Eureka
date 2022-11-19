#include "pch.h"
#include "PowerReceiver.h"

#include "game/inventions/invention.h"
#include "game/inventions/parts/Parts.h"
#include "game/inventions/parts/PowerSupplier.h"
#include "game/inventions/WireConnection.h"

PowerReceiver::PowerReceiver()
	: m_IsPowered(false)
{
}

PowerReceiver::~PowerReceiver()
{
}

void PowerReceiver::SetPoweredState(bool state)
{
	m_IsPowered = state;
}

void PowerReceiver::UpdatePower(Part* part, Invention* invention)
{
	Grid3Pos gridPosition = Grid3Pos::FromVec3(part->GetPosition());
	std::vector<Part*> connectingParts = part->GetConnectingParts(invention);
	bool foundPower = false;
	for (Part* otherPart : connectingParts)
	{
		if (!otherPart) continue;
		if (otherPart->GetPartType() == "empty") continue;
		if (otherPart->GetPartType() == "wire")
		{
			pWire* wire = dynamic_cast<pWire*>(otherPart);
			WireConnection* wireConnection = wire->GetWireConnection();
			if (wireConnection->IsPowered())
				foundPower = true;
			continue;
		}
		if (PowerSupplier* powerSupplier = dynamic_cast<PowerSupplier*>(otherPart))
		{
			if (powerSupplier->IsPowered())
				foundPower = true;
		}
	}
	SetPoweredState(foundPower); // Set true if direct power was found, set false if not
}
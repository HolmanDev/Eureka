#include "pch.h"
#include "WireConnection.h"

#include "game/inventions/invention.h"

WireConnection::WireConnection()
	: m_IsPowered(false)
{
}

WireConnection::~WireConnection()
{
}

void WireConnection::SetPoweredState(bool state)
{
	m_IsPowered = state;
	for (pWire* wire : m_WireParts)
	{
		wire->SetPoweredState(state);
	}
}

void WireConnection::SetWireParts(const std::vector<pWire*>& wireParts)
{
	m_WireParts = wireParts;
}

void WireConnection::AddWirePart(pWire* wirePart)
{
	m_WireParts.push_back(wirePart);
}

void WireConnection::RemoveWirePart(pWire* wirePart)
{
	auto end = std::remove_if(m_WireParts.begin(), m_WireParts.end(), [&](pWire* iWirePart) { return iWirePart == wirePart; });
	m_WireParts.resize(end - m_WireParts.begin());
	if (m_WireParts.empty())
	{
		delete this;
	}
}

void WireConnection::BuildConnections(Invention* invention)
{
	if (m_WireParts.size() > 0)
	{
		BuildConnectionsFromWirePart(m_WireParts[0], invention);
	}
	else
	{
		LOG_WARNING("Couldn't build connections because there were no wire parts added!");
	}
}

void WireConnection::BuildConnectionsFromWirePart(pWire* wirePart, Invention* invention)
{
	std::vector<Grid3Pos> path;
	wirePart->FindConnections(path, invention);
	std::vector<pWire*> oldWireParts = m_WireParts;
	m_WireParts.clear();
	for (const Grid3Pos& gridPosition : path)
	{
		Part* part = invention->SamplePartSpace(gridPosition);
		if (part->GetPartType() != "wire") continue;
		pWire* wire = dynamic_cast<pWire*>(part);
		wire->SetWireConnection(this);
		m_WireParts.push_back(wire);
	}
	for (pWire* wirePart : oldWireParts)
	{
		if (!wirePart)
		{
			LOG_WARNING("Pointer to wire part in oldWireParts is dangling!");
			continue;
		}
		Grid3Pos gridPosition = Grid3Pos::FromVec3(wirePart->GetPosition());
		bool isDisconnected = std::find(path.begin(), path.end(), gridPosition) == path.end();
		if (isDisconnected)
		{
			if (wirePart->GetWireConnection() == this)
			{
				WireConnection* newWireConnection = new WireConnection(); // Must self destruct in the future
				newWireConnection->BuildConnectionsFromWirePart(wirePart, invention);
			}
		}
	}
}

void WireConnection::UpdatePoweredState(Invention* invention)
{
	bool foundPower = false;
	std::vector<Part*> receiversToUpdate;
	for(pWire* wire : m_WireParts)
	{
		std::vector<Part*> connectingParts = wire->GetConnectingParts(invention);
		for (Part* part : connectingParts)
		{
			if (PowerReceiver* powerReceiver = dynamic_cast<PowerReceiver*>(part))
			{
				receiversToUpdate.push_back(part);
			}
			if (PowerSupplier* powerSupplier = dynamic_cast<PowerSupplier*>(part))
			{
				if (powerSupplier->IsPowered())
					foundPower = true;
			}
		}
	}
	SetPoweredState(foundPower);
	for (Part* part : receiversToUpdate)
	{
		PowerReceiver* powerReceiver = dynamic_cast<PowerReceiver*>(part);
		powerReceiver->UpdatePower(part, invention);
	}
}

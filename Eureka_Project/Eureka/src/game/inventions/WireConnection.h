#pragma once

#include "game/inventions/parts/pWire.h"

class invention;

//! Are these power suppliers? And receivers?
//! Should the wire connection be the part and not the wire segments?
class WireConnection
{
private:
	std::vector<pWire*> m_WireParts;
	bool m_IsPowered;
public:
	WireConnection();
	~WireConnection();

	bool IsPowered() const { return m_IsPowered; }
	virtual void SetPoweredState(bool state);

	std::vector<pWire*> GetWireParts() const { return m_WireParts; }
	void SetWireParts(const std::vector<pWire*>& wireParts);
	void AddWirePart(pWire* wirePart);
	void RemoveWirePart(pWire* wirePart);
	void BuildConnections(Invention* invention);
	void BuildConnectionsFromWirePart(pWire* wirePart, Invention* invention);
	void UpdatePoweredState(Invention* invention);
};
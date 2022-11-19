#pragma once

#include "game/inventions/Part.h"
#include "engine/concepts/Grid3D.h"

class pWire;

//! This should also inherit from part.. of course!
class PowerReceiver
{
private:
	bool m_IsPowered;
public:
	PowerReceiver();
	virtual ~PowerReceiver();

	bool IsPowered() const { return m_IsPowered; }
	virtual void SetPoweredState(bool state);

	void UpdatePower(Part* part, Invention* invention);
};
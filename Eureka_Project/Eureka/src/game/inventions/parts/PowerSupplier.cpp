#include "pch.h"
#include "PowerSupplier.h"

PowerSupplier::PowerSupplier()
	: m_IsPowered(false)
{
}

PowerSupplier::~PowerSupplier()
{
}

void PowerSupplier::SetPoweredState(bool state)
{
	m_IsPowered = state;
}

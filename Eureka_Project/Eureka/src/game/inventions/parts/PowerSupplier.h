#pragma once

class PowerSupplier
{
private:
	bool m_IsPowered;
public:
	PowerSupplier();
	virtual ~PowerSupplier();

	bool IsPowered() const { return m_IsPowered; }
	void SetPoweredState(bool state);
};
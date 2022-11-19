#pragma once

#include "game/inventions/Part.h"
#include "PowerSupplier.h"
#include "PowerReceiver.h"

class pPowerSource : public PowerSupplier, public Part
{
private:
	int m_FaceTex;
public:
	pPowerSource();
	~pPowerSource();

	std::string GetPath() override;
	float* GetUVs() override;

	void Action(Invention* invention) override;
};
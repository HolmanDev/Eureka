#pragma once

#include "game/inventions/Part.h"
#include "game/inventions/parts/PowerReceiver.h"

class pLight : public PowerReceiver, public Part
{
private:
	int m_Face;
	int m_PoweredFace;
public:
	pLight();
	~pLight();

	std::string GetPath() override;
	float* GetUVs() override;

	void Action(Invention* invention) override;
	void OnEnvironmentChange(Invention* invention) override;
};
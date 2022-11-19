#pragma once

#include "game/inventions/Part.h"

class pStructure : public Part
{
private:
	int m_FaceTex;
public:
	pStructure();
	~pStructure();

	std::string GetPath() override;
	float* GetUVs() override;

	void Action(Invention* invention) override;
};
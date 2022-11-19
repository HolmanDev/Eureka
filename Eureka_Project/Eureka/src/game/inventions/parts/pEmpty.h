#pragma once

#include "game/inventions/Part.h"

class pEmpty : public Part
{
public:
	pEmpty();
	~pEmpty();

	std::string GetPath() override;

	void Action(Invention* invention) override;
};
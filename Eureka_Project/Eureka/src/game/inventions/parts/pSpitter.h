#pragma once

#include "engine/concepts/Direction.h"
#include "game/inventions/Part.h"
#include "PowerSupplier.h"
#include "PowerReceiver.h"

class pSpitter : public PowerReceiver, public Part
{
private:
	Direction m_Direction;
	int m_SideFace;
	int m_FrontFace;
	int m_BackFace;
public:
	pSpitter();
	~pSpitter();

	std::string GetPath() override;
	float* GetUVs() override;
	Direction GetDirection() const { return m_Direction; }

	void SetDirection(Direction direction);

	void Spit(Invention* invention);

	void Action(Invention* invention) override;
	void OnEnvironmentChange(Invention* invention) override;
	void Configure(Player* player, Invention* invention) override;
	void StopConfiguring(Player* player, Invention* invention) override;
};
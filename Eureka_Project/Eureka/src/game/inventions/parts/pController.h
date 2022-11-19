#pragma once

#include "game/player/Player.h"
#include "game/inventions/Part.h"
#include "PowerSupplier.h"

class pController : public PowerSupplier, public Part
{
private:
	bool m_IsEngaged;
	Player* m_Player;
	int m_KeyCode;
	int m_XFace;
	int m_YFace;
	int m_ZFace;
public:
	pController();
	~pController();

	std::string GetPath() override;
	float* GetUVs() override;
	Player* const GetPlayer() const { return m_Player; }
	int GetKeyCode() const { return m_KeyCode; }

	void SetPlayer(Player* player);
	void SetKeyCode(unsigned int keyCode);

	bool IsEngaged() const { return m_IsEngaged; }
	void SetEngagedState(bool state);

	void Action(Invention* invention) override;
	void Interact(Player* player, Invention* invention) override;
	void Configure(Player* player, Invention* invention) override;
	void StopConfiguring(Player* player, Invention* invention) override;
private:
	void ControlPlayer(Player* player, Invention* invention);
};
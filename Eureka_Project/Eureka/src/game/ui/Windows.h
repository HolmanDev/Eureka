#pragma once

#include "imgui/imgui.h"

#include "engine/ui/Window.h"
#include "game/inventions/parts/Parts.h"

class SpitterConfigWindow : public Window
{
private:
	pSpitter* m_Spitter;
	Player* m_Player;
	bool m_JustOpened;
public:
	SpitterConfigWindow(Player* player, pSpitter* spitter);
	~SpitterConfigWindow();

	pSpitter* GetSpitter() const { return m_Spitter; }

	void SetSpitter(pSpitter* spitter);

	void Run() override;
};

class ControllerConfigWindow : public Window
{
private:
	pController* m_Controller;
	bool m_JustOpened;
	bool m_ChoosingKey;
public:
	ControllerConfigWindow(pController* controller);
	~ControllerConfigWindow();

	pController* GetController() const{ return m_Controller; }

	void SetController(pController* controller);

	void Run() override;
};

class PauseWindow : public Window
{
public:
	PauseWindow();
	~PauseWindow();

	void Run() override;
};
#pragma once

#include "engine/ui/Window.h"

class Player;
class pSpitter;
class pController;

// Responsible for displaying UI
class UIManager
{
private:
	static std::list<std::shared_ptr<Window>> m_Windows;
	static std::list<Window*> m_WindowsToClose;
public:
	static void Init();
	static void Run();
	static void CloseWindow(Window* window);

	static std::shared_ptr<Window> Pause();
	static std::shared_ptr<Window> SpitterConfig(Player* player, pSpitter* spitter);
	static std::shared_ptr<Window> ControllerConfig(pController* controller);
};
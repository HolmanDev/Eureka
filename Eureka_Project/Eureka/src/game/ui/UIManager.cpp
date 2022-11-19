#include "pch.h"
#include "UIManager.h"

#include "game/ui/Windows.h"
#include "game/player/Player.h"
#include "game/inventions/parts/Parts.h"

std::list<std::shared_ptr<Window>> UIManager::m_Windows;
std::list<Window*> UIManager::m_WindowsToClose;

void UIManager::Init()
{
	//Pause();
}

void UIManager::Run()
{
	for (std::shared_ptr<Window> window : m_Windows)
	{
		window->Run();
	}
	for (Window* window : m_WindowsToClose)
	{
		m_Windows.remove_if([&](std::shared_ptr<Window> iWindow) { return iWindow.get() == window; });
	}
	m_WindowsToClose.clear();
}

void UIManager::CloseWindow(Window* window)
{
	m_WindowsToClose.push_back(window);
}

std::shared_ptr<Window> UIManager::Pause()
{
	std::shared_ptr<Window> window = std::make_shared<PauseWindow>();
	m_Windows.push_back(window);
	return window;
}

std::shared_ptr<Window> UIManager::SpitterConfig(Player* player, pSpitter* spitter)
{
	std::shared_ptr<Window> window = std::make_shared<SpitterConfigWindow>(player, spitter);
	m_Windows.push_back(window);
	return window;
}

std::shared_ptr<Window> UIManager::ControllerConfig(pController* controller)
{
	std::shared_ptr<Window> window = std::make_shared<ControllerConfigWindow>(controller);
	m_Windows.push_back(window);
	return window;
}

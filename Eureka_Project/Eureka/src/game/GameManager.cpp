#include "pch.h"
#include "GameManager.h"

#include "game/ui/UIManager.h"

bool GameManager::s_Paused = false;
float GameManager::s_Sensitivity = 0.1f;
World* GameManager::s_World = nullptr;

void GameManager::SetSensitivity(float sensitivity)
{
	s_Sensitivity = sensitivity;
}

void GameManager::SetWorld(World* world)
{
	s_World = world;
}

bool GameManager::IsPaused()
{
	return s_Paused;
}

void GameManager::Pause()
{
	s_Paused = true;
	UIManager::Pause();
}

void GameManager::Unpause()
{
	s_Paused = false;
}

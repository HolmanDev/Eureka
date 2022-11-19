#include "pch.h"
#include "Windows.h"

#include "engine/core/InputManager.h"
#include "game/GameManager.h"
#include "game/ui/UIManager.h"

#pragma region Spitter Config
SpitterConfigWindow::SpitterConfigWindow(Player* player, pSpitter* spitter)
	: m_Spitter(spitter), m_Player(player), m_JustOpened(false)
{
}

SpitterConfigWindow::~SpitterConfigWindow()
{
}

void SpitterConfigWindow::SetSpitter(pSpitter* spitter)
{
	m_Spitter = spitter;
}

void SpitterConfigWindow::Run()
{
	if (!m_JustOpened && InputManager::KeyPressedDown(GLFW_KEY_ESCAPE))
	{
		UIManager::CloseWindow(this);
		m_Spitter->StopConfiguring(m_Player, nullptr);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(380, 55));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Spitter", NULL, flags);

	static const char* directions[]{ "Right", "Left", "Up", "Down", "Forward", "Backward" };
	int selectedDirection = (int) m_Spitter->GetDirection();

	if (ImGui::Combo("Choose direction", &selectedDirection, directions, 6))
	{
		Direction direction = DirectionHandler::ToDirection(selectedDirection);
		m_Spitter->SetDirection(direction);
	}
	ImGui::End();
	m_JustOpened = false;
}
#pragma endregion

#pragma region Controller Config
ControllerConfigWindow::ControllerConfigWindow(pController* controller)
	: m_Controller(controller), m_JustOpened(true), m_ChoosingKey(false)
{
}

ControllerConfigWindow::~ControllerConfigWindow()
{
}

void ControllerConfigWindow::SetController(pController* controller)
{
	m_Controller = controller;
}

void ControllerConfigWindow::Run()
{
	Player* player = m_Controller->GetPlayer();
	if (!m_JustOpened && InputManager::KeyPressedDown(GLFW_KEY_ESCAPE))
	{
		UIManager::CloseWindow(this);
		m_ChoosingKey = false;
		if (player) player->SetTyping(false);
		m_Controller->StopConfiguring(m_Controller->GetPlayer(), nullptr);
		return;
	}

	if (InputManager::KeyPressedDown(GLFW_MOUSE_BUTTON_1) || InputManager::KeyPressedDown(GLFW_KEY_ENTER))
	{
		m_ChoosingKey = false;
		if (player) player->SetTyping(false);
	}

	ImGui::SetNextWindowSize(ImVec2(215, 60));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse 
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Controller", NULL, flags);

	int key = InputManager::GetLastPressed();
	char buttonText[128] = "Choose key: ";
	if (key >= 0)
	{
		char keyName[128] = "Unknown";
		if (m_ChoosingKey)
		{
			m_Controller->SetKeyCode(key);
			const char* x = glfwGetKeyName(key, glfwGetKeyScancode(key));
			if(x) strcpy_s(keyName, x);
		}
		else
		{
			int oldKey = m_Controller->GetKeyCode();
			const char* x = glfwGetKeyName(oldKey, glfwGetKeyScancode(oldKey));
			if (x) strcpy_s(keyName, x);
		}
		strcat_s(buttonText, keyName);
	}
	else
	{
		strcat_s(buttonText, "Unknown");
	}
	// Key selection button
	if (m_ChoosingKey)
	{
		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
	}
	if (ImGui::Button(buttonText, ImVec2(200, 25)))
	{
		m_ChoosingKey = true;
		if (player) player->SetTyping(true);
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();
	m_JustOpened = false;
}
#pragma endregion

#pragma region Pause Window
PauseWindow::PauseWindow()
{
}

PauseWindow::~PauseWindow()
{
}

void PauseWindow::Run()
{
	if (!GameManager::IsPaused())
	{
		UIManager::CloseWindow(this);
		return;
	}

	static float sensitivity = 0.1f;

	ImGui::Begin("Paused");

	ImGui::Text("Take your time and relax ");
	ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 3.0f);
	GameManager::SetSensitivity(sensitivity);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}
#pragma endregion
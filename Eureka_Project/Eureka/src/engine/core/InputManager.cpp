#include "pch.h"
#include "InputManager.h"

GLFWwindow* InputManager::s_Window;
bool InputManager::m_PressedButtons[1000];
std::vector<int> InputManager::m_NewPressedButtons;
std::vector<int> InputManager::m_NewReleasedButtons;
glm::vec2 InputManager::m_MousePos = glm::vec2(0, 0);
glm::vec2 InputManager::m_LastMousePos = glm::vec2(0, 0);
glm::vec2 InputManager::m_MouseDelta = glm::vec2(0, 0);
int InputManager::s_LastPressed = -1;

void InputManager::SetWindow(GLFWwindow* window)
{
	s_Window = window;
}

// Must be called BEFORE EventManager::Update
void InputManager::Update()
{
	for (int key : m_NewReleasedButtons) {
		m_PressedButtons[key] = false;
	}

	for (int key : m_NewPressedButtons) {
		m_PressedButtons[key] = true;
	}
}

// Must be called AFTER EventManager::Update
void InputManager::Reset()
{
	m_NewPressedButtons.clear();
	m_NewReleasedButtons.clear();

	m_MouseDelta = m_MousePos - m_LastMousePos;
	m_LastMousePos = m_MousePos;
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		s_LastPressed = key;
		m_NewPressedButtons.push_back(key);
	}
	else if (action == GLFW_RELEASE)
	{
		m_NewReleasedButtons.push_back(key);
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		s_LastPressed = button;
		m_NewPressedButtons.push_back(button); //! Will this clash with the keycodes?
	}
	else if (action == GLFW_RELEASE)
	{
		m_NewReleasedButtons.push_back(button);
	}
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	m_MousePos = glm::vec2(xPos, yPos);
}

bool InputManager::KeyPressed(int key)
{
	return m_PressedButtons[key];
}

bool InputManager::KeyPressedDown(int key)
{
	for (int qkey : m_NewPressedButtons) {
		if (qkey == key)
			return true;
	}
	return false;
}

bool InputManager::KeyPressedUp(int key)
{
	for (int qkey : m_NewReleasedButtons) {
		if (qkey == key)
			return true;
	}
	return false;
}

void InputManager::InitMousePos(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	m_MousePos = glm::vec2(xpos, ypos);
	m_LastMousePos = m_MousePos;
}

void InputManager::SetCursorMode(int mode)
{
	glfwSetInputMode(s_Window, GLFW_CURSOR, mode);
}

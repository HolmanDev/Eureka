#pragma once

#include "engine/glCore.h"

class InputManager
{
private:
	static GLFWwindow* s_Window;
	static std::vector<int> m_NewPressedButtons;
	static std::vector<int> m_NewReleasedButtons;
	static bool m_PressedButtons[];
	static glm::vec2 m_MousePos;
	static glm::vec2 m_LastMousePos;
	static glm::vec2 m_MouseDelta;
	static int s_LastPressed;
public:
	static const GLFWwindow* GetWindow() { return s_Window; }
	static void SetWindow(GLFWwindow* window);

	static void Update();
	static void Reset();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static bool KeyPressed(int key);
	static bool KeyPressedDown(int key);
	static bool KeyPressedUp(int key);
	static const int GetLastPressed() { return s_LastPressed; }
	static void InitMousePos(GLFWwindow* window);
	static const inline glm::vec2 GetMousePos() { return m_MousePos; }
	static const inline glm::vec2 GetMouseDelta() { return m_MouseDelta; }
	static void SetCursorMode(int mode);
};
#pragma once

class World;

class GameManager 
{
private:
	static bool s_Paused;
	static float s_Sensitivity;
	static World* s_World;
public:
	static float GetSensitivity() { return s_Sensitivity; }
	static World* GetWorld() { return s_World; }

	static void SetSensitivity(float sensitivity);
	static void SetWorld(World* world);

	static bool IsPaused();
	static void Pause();
	static void Unpause();
};
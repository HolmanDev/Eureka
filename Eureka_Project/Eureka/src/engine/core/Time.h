#pragma once

#include "pch.h"

class Time
{
private:
	static float m_Time;
	static std::chrono::high_resolution_clock::time_point m_StartTime;
	static float m_Timestep;
public:
	static void Start();
	static void Update();
	static void UpdateTimestep(); //! Does this return the current or last timestep?

	inline static const float GetTime() { return m_Time; }
	inline static const float GetTimestep() { return m_Timestep; }
};
#include "pch.h"
#include "Time.h"

float Time::m_Time = 0.0f;
std::chrono::high_resolution_clock::time_point Time::m_StartTime = std::chrono::high_resolution_clock::time_point();
float Time::m_Timestep = 0.0f;

void Time::Start()
{
	m_Time = 0.0f;
	m_StartTime = std::chrono::high_resolution_clock::now();
}

void Time::Update()
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
	m_Time = duration.count();
}

void Time::UpdateTimestep()
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
	m_Timestep = duration.count() - m_Time;
}

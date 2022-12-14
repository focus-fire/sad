#include "sadpch.h"

#include "Time.h"

#include <SDL2/SDL.h>

float pog::Time::TimeScale = 1.0f;
float pog::Time::m_StartTime = 0.0f;

float pog::Time::GetDeltaTime()
{
	return CalculateDeltaTime();
}

float pog::Time::CalculateDeltaTime()
{
	float ticks = static_cast<float>(SDL_GetTicks());

	float dt = (ticks - m_StartTime) * TimeScale / 1000.0f;
	m_StartTime = ticks;

	return dt;
}

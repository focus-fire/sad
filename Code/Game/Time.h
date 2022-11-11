#pragma once

namespace pog
{
	class Time final
	{
	private:
		static float CalculateDeltaTime();

	public:
		/**
		 * @brief Retrieves the current frame calculation for deltaTime
		*/
		static float GetDeltaTime();

		/**
		 * @brief Scales the calculation for time in the editor by it's value 
		*/
		static float TimeScale;

	private:
		static float m_StartTime;
	};
}

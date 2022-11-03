#pragma once

namespace sad
{
	/**
	 * @brief The number people learn to recite for fun
	*/
	const double c_PI  = 3.1415926535897932385;

	/**
	 * @brief The number people learn to recite for fun divided by two
	*/
	const double c_PI2 = (c_PI / 2);

	/**
	 * @brief Converts from degrees to radians
	 * @param deg Number representing degrees 
	 * @return Number representing degrees converted to radians
	*/
	inline float DegToRad(float deg) { return deg * c_PI / 180.0f; }

	/**
	 * @brief Clamps an input number between a desired minimum or maximum
	 * @param x Input number to be clamped
	 * @param min Desired minimum floor value
	 * @param max Desired maximum ceiling value
	 * @return Number clamped between the minimum and maximum
	*/
	inline float Clamp(float x, float min, float max)
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
}

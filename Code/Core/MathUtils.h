#pragma once

#include <random>

/**
 * @brief The number people learn to recite for fun
*/
const double c_PI  = 3.1415926535897932385;

/**
 * @brief The number people learn to recie for fun multiplied by two
*/
const double c_2PI = (c_PI * 2);

/**
 * @brief The number people learn to recite for fun divided by two
*/
const double c_PI2 = (c_PI / 2);

namespace core 
{
	/**
	 * @brief Utilities for performing math operations
	*/
	class MathUtils
	{
	public:
		/**
		 * @brief Rounds a passed float to the nearest value with the indicated precision
		 * @param value Floating-point value to round
		 * @param precision Floating-point value representing the precision for rounding (ex: 0.1 = nearest tenth, 0.01 = nearest hundredth, 0.001 = nearest thousandth etc.)
		 * @return Rounded floating-point value with the indicated precision
		*/
		static double RoundTo(double value, double precision = 1.0f);

		/**
		 * @brief Compares floating-point numbers with an epsilon (since floating-point math isn't exact)
		 * @param a First double for comparison
		 * @param b Second double for comparison 
		 * @param epsilon Value to evaluate error 
		 * @return True if the doubles are the same, false if they are different
		 * @reference https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
		 * @reference http://realtimecollisiondetection.net/blog/?p=89
		*/
		static bool CompareFloatingPoints(double a, double b, double epsilon = 0.001);

		/**
		 * @brief Converts from degrees to radians
		 * @param deg Number representing degrees 
		 * @return Number representing degrees converted to radians
		*/
		static float DegToRad(float deg);

		/**
		 * @brief Clamps an input number between a desired minimum or maximum
		 * @param x Input number to be clamped
		 * @param min Desired minimum floor value
		 * @param max Desired maximum ceiling value
		 * @return Number clamped between the minimum and maximum
		*/
		static float Clamp(float x, float min, float max);

		/**
		 * @brief Generates a pseudo-random number between a specified min and max
		 * @param min Minimum bound for random number
		 * @param mx Maximum bound for random number 
		 * @return Generated pseudo-random number
		*/
		static int RandomBetween(int min, int max);

	private:
		static std::random_device m_RandomDevice;
		static std::mt19937 m_RandomGenerator;
	};
}

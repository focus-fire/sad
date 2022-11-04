#include "sadpch.h"

#include "MathUtils.h"

double core::MathUtils::RoundTo(double value, double precision /* = 1.0f */)
{
	return std::round(value / precision) * precision;
}

bool core::MathUtils::CompareFloatingPoints(double a, double b, double epsilon /* = 0.001 */)
{
	return fabs(a - b) < epsilon;
}

float core::MathUtils::DegToRad(float deg) 
{ 
	return static_cast<float>(deg * c_PI / 180.0f); 
}

float core::MathUtils::Clamp(float x, float min, float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


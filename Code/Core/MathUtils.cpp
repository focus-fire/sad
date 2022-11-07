#include "sadpch.h"

#include "MathUtils.h"

std::random_device core::MathUtils::m_RandomDevice;
std::mt19937 core::MathUtils::m_RandomGenerator = std::mt19937(core::MathUtils::m_RandomDevice());

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

int core::MathUtils::RandomBetween(int min, int max)
{
	if (min == max)
		return min;

	std::uniform_int_distribution<int> distribution = std::uniform_int_distribution<int>(min, max);
	return distribution(m_RandomGenerator);
}

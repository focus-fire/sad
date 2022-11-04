#pragma once
#include <string>

namespace core
{
	class StringUtils
	{
	public:
		static std::string LeftTrim(const std::string& s);
		static std::string RightTrim(const std::string& s);
	};
}
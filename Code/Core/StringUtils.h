#pragma once
#include <string>

namespace core
{
	/**
	 * @brief Various utilities and helpers for working with strings
	*/
	class StringUtils
	{
	public:
		/**
		 * @brief Trims leading and trailing white-spaces
		 * @param s String to trim leading and trailing white-spaces from
		*/
		static std::string Trim(const std::string& str);

		/**
		 * @brief Trim leading white-spaces
		 * @param s String to trim leading white-spaces from
		*/
		static std::string LeftTrim(const std::string& str);


		/**
		 * @brief Trim trailing white-spaces
		 * @param s String to trim trailing white-spaces from
		*/
		static std::string RightTrim(const std::string& str);
	};
}
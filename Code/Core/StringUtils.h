#pragma once

#include <string>
#include <vector>

namespace core
{
	/**
	 * @brief Various utilities and helpers for working with strings
	*/
	class StringUtils
	{
	public:
		/**
		 * @brief Compares two strings for equality
		 * @return True if strings are the same, false if they are different
		*/
		static bool Equals(const std::string& a, const std::string& b);

		/**
		 * @brief Checks if string b is contained in string a 
		 * @return True if string b exists in a, false if it does not
		*/
		static bool Contains(const std::string& a, const std::string& b);

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

		/**
		 * @brief Splits a string by a provided delimiter
		 * @param delimiter Character used as the basis for splitting the string 
		 * @param str String to split
		 * @return Vector containing each string split during the operation
		*/
		static std::vector<std::string> Split(const char delimiter, const std::string& str);
	};
}
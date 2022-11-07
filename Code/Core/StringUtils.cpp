#include "sadpch.h"

#include "StringUtils.h"

bool core::StringUtils::Equals(const std::string& a, const std::string& b)
{
    return a.compare(b) == 0; 
}

std::string core::StringUtils::Trim(const std::string& str)
{
    std::string trimmedString = str;

    trimmedString = LeftTrim(trimmedString);
    trimmedString = RightTrim(trimmedString);

    return trimmedString;
}

std::string core::StringUtils::LeftTrim(const std::string& str)
{
    std::string tempString = str;
    size_t startPos = tempString.find_first_not_of(" \t\r\n\v\f");

    if (std::string::npos != startPos)
    {
        tempString = tempString.substr(startPos);
    }

    return tempString;
}

std::string core::StringUtils::RightTrim(const std::string& str)
{
    std::string tempString = str;
    size_t endPos = tempString.find_last_not_of(" \t\r\n\v\f");

    if (std::string::npos != endPos)
    {
        tempString = tempString.substr(0, endPos + 1);
    }

    return tempString;
}
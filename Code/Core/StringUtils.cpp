#include "sadpch.h"

#include "StringUtils.h"

bool core::StringUtils::Equals(const std::string& a, const std::string& b)
{
    return a.compare(b) == 0; 
}

bool core::StringUtils::Contains(const std::string& a, const std::string& b)
{
    return a.find(b) != std::string::npos;
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

std::vector<std::string> core::StringUtils::Split(const char delimiter, const std::string& str)
{
    std::vector<std::string> result;
    std::string element;

    size_t nextPosition = 0;
    size_t lastPosition = 0;

    // Iterate through the string and locate the delimiter
    while ((nextPosition = str.find(delimiter, lastPosition)) != std::string::npos)
    {
        element = str.substr(lastPosition, nextPosition - lastPosition);
        result.push_back(element);

        // Increment position to check next applicable substring
        lastPosition = nextPosition + 1;
    }

    // Catch whatever remains after the final delimiter when the loop breaks
    result.push_back(str.substr(lastPosition));

    return result;
}

#include "sadpch.h"

#include "StringUtils.h"

/**
 * @brief trim leading white-spaces
 * @param s
 * @return
*/
std::string core::StringUtils::LeftTrim(const std::string& s)
{
    std::string tempString = s;
    size_t startpos = tempString.find_first_not_of(" \t\r\n\v\f");
    if (std::string::npos != startpos)
    {
        tempString = tempString.substr(startpos);
    }
    return tempString;
}

/**
 * @brief trim trailing white-spaces
 * @param s
 * @return
*/
std::string core::StringUtils::RightTrim(const std::string& s)
{
    std::string tempString = s;
    size_t endpos = tempString.find_last_not_of(" \t\r\n\v\f");
    if (std::string::npos != endpos)
    {
        tempString = tempString.substr(0, endpos + 1);
    }
    return tempString;
}
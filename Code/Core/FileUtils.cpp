#include "sadpch.h"

#include "FileUtils.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <filesystem>

/**
 * @brief 
 * Returns the file contents as a string
 * @param path 
 * @return 
*/
std::string core::FileUtils::ReadFile(std::string path)
{
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		std::stringstream buffer;
		while (getline(myfile, line))
		{
			buffer << line;
		}
		myfile.close();
		return buffer.str();
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
	return "";
}

/**
 * @brief 
 * Creates a new file with given contents
 * @param path 
 * @param content 
*/
void core::FileUtils::CreateNewFile(std::string path, std::string content)
{
	std::ofstream myfile(path);
	if (myfile.is_open())
	{
		core::Log(ELogType::Trace, "File Opened {}", path);
		myfile << content << std::endl;
		myfile.close();
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
}

/**
 * @brief 
 * Rewrites the entire file of its contents
 * @param path 
 * @param content 
*/
void core::FileUtils::OverWriteExistingFile(std::string path, std::string content)
{
	std::fstream myfile;
	myfile.open(path, std::ios_base::out | std::ios_base::in); // will not create a file 
	if (myfile.is_open())
	{
		core::Log(ELogType::Trace, "File Opened {}", path);
		myfile << content << std::endl;
		myfile.close();
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
}

/**
 * @brief 
 * Append content to existing file
 * @param path 
 * @param content 
*/
void core::FileUtils::AppendToExistingFile(std::string path, std::string content)
{
	std::ofstream outfile;
	outfile.open(path, std::ios_base::app); // append instead of overwrite
	if (outfile.is_open())
	{
		outfile << content << std::endl;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
}

/**
 * @brief 
 * Gets sad directory path
 * @return 
*/
std::string core::FileUtils::GetProjectDirectory()
{
	std::string projectDirectory = std::filesystem::current_path().string();

#ifdef _SAD_WINDOWS
	std::size_t codeDirectory = projectDirectory.find_last_of("/\\");
	projectDirectory = projectDirectory.substr(0, codeDirectory);
#endif

	return projectDirectory;
}

/**
 * @brief 
 * Gets data directory path
 * @return 
*/
std::string core::FileUtils::GetDataDirectory()
{
	std::string dataDirectory = GetProjectDirectory();
	dataDirectory += ConvertOSPathString("/Data");

	return dataDirectory;
}

/**
 * @brief 
 * return the correct path based on operating system
 * @param path 
 * Only pass '/' for pathing slashes
 * @return 
*/
std::string core::FileUtils::ConvertOSPathString(std::string path)
{
	std::string convertedPath = path;
#ifdef _SAD_WINDOWS
	replace(convertedPath.begin(), convertedPath.end(), '/', '\\');
#endif
	
	return convertedPath;
}
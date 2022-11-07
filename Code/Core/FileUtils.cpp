#include "sadpch.h"

#include "FileUtils.h"
#include <stdlib.h>

/**
 * @brief 
 * Returns the file contents as a string
 * @param path 
 * @return 
*/
std::string core::FileUtils::ReadFile(const std::string& path)
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
bool core::FileUtils::CreateNewFile(const std::string& path, const std::string& content)
{
	std::ofstream myfile(path);
	if (myfile.is_open())
	{
		core::Log(ELogType::Trace, "File Opened {}", path);
		myfile << content << std::endl;
		myfile.close();
		return true;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
	return false;
}

/**
 * @brief 
 * Rewrites the entire file of its contents
 * @param path 
 * @param content 
*/
bool core::FileUtils::OverWriteExistingFile(const std::string& path, const std::string& content)
{
	std::fstream myfile;
	myfile.open(path, std::ios_base::out | std::ios_base::in); // will not create a file 
	if (myfile.is_open())
	{
		core::Log(ELogType::Trace, "File Opened {}", path);
		myfile << content << std::endl;
		myfile.close();
		return true;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
	return false;
}

/**
 * @brief 
 * Append content to existing file
 * @param path 
 * @param content 
*/
bool core::FileUtils::AppendToExistingFile(const std::string& path, const std::string& content)
{
	std::ofstream outfile;
	outfile.open(path, std::ios_base::app); // append instead of overwrite
	if (outfile.is_open())
	{
		outfile << content << std::endl;
		return true;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);
	return false;
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


std::string core::FileUtils::GetCodeDirectory()
{
	std::string codeDirectory = GetProjectDirectory();
	codeDirectory += ConvertOSPathString("/Code");

	return codeDirectory;
}

/**
 * @brief 
 * return the correct path based on operating system
 * @param path 
 * Only pass '/' for pathing slashes
 * @return 
*/
std::string core::FileUtils::ConvertOSPathString(const std::string& path)
{
	std::string convertedPath = path;
#ifdef _SAD_WINDOWS
	replace(convertedPath.begin(), convertedPath.end(), '/', '\\');
#endif
	
	return convertedPath;
}
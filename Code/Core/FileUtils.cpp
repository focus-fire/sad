#include "sadpch.h"

#include "FileUtils.h"

#include <stdlib.h>

std::string core::FileUtils::ReadFile(const std::string& path)
{
	std::string line;
	std::ifstream fileStream = std::ifstream(path);

	if (fileStream.is_open())
	{
		std::stringstream buffer;
		while (getline(fileStream, line))
		{
			std::cout << "BRO" << line << std::endl;
			buffer << line;
		}
		fileStream.close();
		return buffer.str();
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);

	return "";
}

bool core::FileUtils::WriteFile(const std::string& path, const std::string& content)
{
	std::ofstream fileStream = std::ofstream(path);

	if (fileStream.is_open())
	{
		core::Log(ELogType::Trace, "File Opened {}", path);
		fileStream << content << std::endl;
		fileStream.close();
		return true;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);

	return false;
}

bool core::FileUtils::AppendFile(const std::string& path, const std::string& content)
{
	std::ofstream outFileStream;
	outFileStream.open(path, std::ios_base::app); // append instead of overwrite

	if (outFileStream.is_open())
	{
		outFileStream << content << std::endl;
		return true;
	}
	else core::Log(ELogType::Trace, "Unable to open file: {}", path);

	return false;
}

bool core::FileUtils::RemoveFile(const std::string& path)
{
	return std::remove(path.c_str()) == 0;
}

std::string core::FileUtils::GetProjectDirectory()
{
	std::string projectDirectory = std::filesystem::current_path().string();

#ifdef _SAD_WINDOWS
	// While running the project on Windows, the current_path() becomes rooted from the 'Code' folder
	// However on other platforms and while running tests, 'sad' remains the return value for current_path
	if (projectDirectory.find("Code") != std::string::npos)
	{
		std::size_t codeDirectory = projectDirectory.find_last_of("/\\");
		projectDirectory = projectDirectory.substr(0, codeDirectory);
	}
#endif

	return projectDirectory;
}

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

std::string core::FileUtils::ConvertOSPathString(const std::string& path)
{
	std::string convertedPath = path;

#ifdef _SAD_WINDOWS
	replace(convertedPath.begin(), convertedPath.end(), '/', '\\');
#endif
	
	return convertedPath;
}
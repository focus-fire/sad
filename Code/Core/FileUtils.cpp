#include "sadpch.h"

#include "FileUtils.h"

#include <stdlib.h>

std::string core::FileUtils::ReadFile(const std::string& path, bool addLineEndings)
{
	std::string line;
	std::string lineBreak = addLineEndings ? "\n" : "";
	std::ifstream fileStream = std::ifstream(path);

	if (fileStream.is_open())
	{
		std::stringstream buffer;
		while (getline(fileStream, line))
		{
			buffer << line << lineBreak;
		}

		fileStream.close();

		// Remove line break added at the end of the file 
		std::string bufferString = buffer.str();
		std::size_t lastLineBreak = bufferString.find_last_of(lineBreak);
		bufferString = bufferString.substr(0, lastLineBreak);

		return bufferString;
	}
	else core::Log(ELogType::Trace, "[FileUtils] Unable to open file: {}", path);

	return "";
}

bool core::FileUtils::WriteFile(const std::string& path, const std::string& content)
{
	std::ofstream fileStream = std::ofstream(path);

	if (fileStream.is_open())
	{
		core::Log(ELogType::Trace, "[FileUtils] {} opened for writing", path);

		fileStream << content << std::endl;
		fileStream.close();

		return true;
	}
	else core::Log(ELogType::Trace, "[FileUtils] Unable to open {}", path);

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
	else core::Log(ELogType::Trace, "[FileUtils] Unable to open {}", path);

	return false;
}

bool core::FileUtils::RemoveFile(const std::string& path)
{
	return std::remove(path.c_str()) == 0;
}

bool core::FileUtils::PathExists(const std::string& path)
{
	return std::filesystem::exists(path);
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
	std::string projectDirectory = std::move(GetProjectDirectory());
	std::string dataDirectory = std::move(ConvertOSPathString("/Data/"));

	return projectDirectory.append(dataDirectory);
}

std::string core::FileUtils::GetCodeDirectory()
{
	std::string projectDirectory = std::move(GetProjectDirectory());
	std::string codeDirectory = std::move(ConvertOSPathString("/Code/"));

	return projectDirectory.append(codeDirectory);
}

std::string core::FileUtils::ConvertOSPathString(const std::string& path)
{
	std::string convertedPath = path;

#ifdef _SAD_WINDOWS
	replace(convertedPath.begin(), convertedPath.end(), '/', '\\');
#endif
	
	return convertedPath;
}

std::string core::FileUtils::GetPathInsideDataDirectory(const std::string& path)
{
	std::string dataDirectory = std::move(GetDataDirectory());
	std::string osPath = std::move(ConvertOSPathString(path));

	return dataDirectory.append(osPath);
}

std::string core::FileUtils::GetPathInsideCodeDirectory(const std::string& path)
{
	std::string codeDirectory = std::move(GetCodeDirectory());
	std::string osPath = std::move(ConvertOSPathString(path));

	return codeDirectory.append(osPath);
}
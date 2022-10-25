#pragma once
#include <string>

namespace core
{
	class FileUtils
	{
	public:
		static std::string ReadFile(std::string path);
		static void CreateNewFile(std::string path, std::string content);
		static void OverWriteExistingFile(std::string path, std::string content);
		static void AppendToExistingFile(std::string path, std::string content);
		static std::string GetProjectDirectory();
		static std::string GetDataDirectory();
		static std::string ConvertOSPathString(std::string path);
	};
}

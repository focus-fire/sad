#pragma once
#include <string>

namespace core
{
	class FileUtils
	{
	public:
		static std::string ReadFile(const std::string& path);
		static bool CreateNewFile(const std::string& path, const std::string& content);
		static bool OverWriteExistingFile(const std::string& path, const std::string& content);
		static bool AppendToExistingFile(const std::string& path, const std::string& content);
		static std::string GetProjectDirectory();
		static std::string GetDataDirectory();
		static std::string GetCodeDirectory();
		static std::string ConvertOSPathString(const std::string& path);
	};
}

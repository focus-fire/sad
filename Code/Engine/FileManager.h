#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace sad
{
	class FileManager
	{
	public:
		FileManager();

		std::string Read(std::string path);
		void JSONRead(std::string path);
		void WriteExisting(std::string path, std::string content);
		void AppendExisting(std::string path, std::string content);
		void CreateWrite(std::string path, std::string content);
	};
}
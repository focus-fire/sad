#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

namespace sad
{
	class FileManager
	{
	public:
		FileManager();

		void Read(std::string path);
		void JSONRead(std::string path);
		void WriteExisting(std::string path, std::string content);
		void AppendExisting(std::string path, std::string content);
		void CreateWrite(std::string path, std::string content);
	};
}
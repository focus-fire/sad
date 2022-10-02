#include "FileManager.h"

sad::FileManager::FileManager()
{
	
}

void sad::FileManager::Read()
{
	std::string line;
	std::ifstream myfile("example.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::cout << line << std::endl;
		}
		myfile.close();
	}

	else std::cout << "Unable to open file";
}

void sad::FileManager::Write()
{
	std::ofstream myfile("./example.txt");
	if (myfile.is_open())
	{
		myfile << "This is a line." << std::endl;
		myfile << "This is another line." << std::endl;
		myfile.close();
	}
	else std::cout << "Unable to open file";
}

int main() {
	return 0;
}
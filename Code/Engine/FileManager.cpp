#include "FileManager.h"

using json = nlohmann::json;
//Constructor
sad::FileManager::FileManager(){}

//File read function 
std::string sad::FileManager::Read(std::string path)
{
	std::string line;
	std::ifstream myfile("C://Users//kiwon//Desktop//SadEngine//sad//Code//Engine//example.json"); //pathing fixing required
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
	else std::cout << "Unable to open file";
	return "";
}

void sad::FileManager::WriteExisting(std::string path, std::string content)
{
	//std::ofstream myfile("example.txt");
	std::fstream myfile;
	myfile.open(path, std::ios_base::out | std::ios_base::in); // will not create a file 
	if (myfile.is_open())
	{
		std::cout << "File Opened" << std::endl;
		myfile << content << std::endl;
		myfile.close();
	}
	else std::cout << "Unable to open file"; //output to debugger?
}

void sad::FileManager::AppendExisting(std::string path, std::string content)
{	
	std::ofstream outfile;
	outfile.open("example.txt", std::ios_base::app); // append instead of overwrite
	if (outfile.is_open())
	{
		outfile << content << std::endl;
	}
	else std::cout << "Unable to open file"; //output to debugger?
}

void sad::FileManager::CreateWrite(std::string path, std::string content)
{
	//std::ofstream myfile("example.txt");
	std::ofstream myfile(path);
	if (myfile.is_open())
	{
		std::cout << "File Opened" << std::endl;
		myfile << content << std::endl;
		myfile.close();
	}
	else std::cout << "Unable to open file"; //output to debugger?
}
#include "sadpch.h"

#include "JsonManager.h"

using json = nlohmann::json;
//Constructor
sad::JsonManager::JsonManager(){}

//File read function 
std::string sad::JsonManager::Read(std::string path)
{
	std::string line;
	std::ifstream myfile("..//Code//Engine//example.json"); //pathing fixing based on OS required
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

void sad::JsonManager::WriteExisting(std::string path, std::string content)
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

void sad::JsonManager::AppendExisting(std::string path, std::string content)
{	
	std::ofstream outfile;
	outfile.open("example.txt", std::ios_base::app); // append instead of overwrite
	if (outfile.is_open())
	{
		outfile << content << std::endl;
	}
	else std::cout << "Unable to open file"; //output to debugger?
}

void sad::JsonManager::CreateWrite(std::string path, std::string content)
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
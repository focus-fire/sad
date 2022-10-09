#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <filesystem>

void generate_config(const std::string& filename, const std::string& sectionname, std::list<std::string>& configToSave)
{    
    std::ofstream ostrm;
    ostrm.open(filename, std::ios_base::app);

    if (ostrm)
    {
        ostrm << sectionname + "\n";
        for (auto& config : configToSave)
        {
            ostrm << config << "\n";
        }
        ostrm << "\n";
    }
    ostrm.close();
}

void empty_config(std::string filename) 
{
    // delete file if it exists
    if (std::filesystem::exists(filename))
    {
        std::filesystem::remove(filename);
    }
    else 
    {
        std::cout << "File " << filename << " does not exist" << std::endl;
    }
}

int main()
{
    std::list<std::string> protocols;
    protocols.push_back("version=6");
    protocols.push_back("active=true");
    protocols.push_back("pi=3.14159");

    std::list<std::string> users;
    users.push_back("email=kik@kik.com");
    users.push_back("name=Bob Smith");

    // generate test file
    empty_config("config.ini");
    generate_config("config.ini", "[protocol]", protocols);
    generate_config("config.ini", "[user]", users);

    // retrieve some information from config file
    section* usersection = ConfigManager::getsection("user");;

    if (usersection != NULL)
    {
        std::cout << "section name: " << usersection->name << std::endl;
        std::cout << "email=" << ConfigManager::getvalue("user", "email") << '\n';
    }
    else
    {
        std::cout << "empty" << std::endl;
    }
}

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <unordered_map>
#include <list>


struct section
{
    std::string name;
    std::unordered_map<std::string, std::string> keyvalues;
};

class ConfigManager
{
public:
    ConfigManager(const std::string& filename);

    section* get_section(const std::string& sectionname);
    std::list<section>& get_sections();

    std::string get_value(const std::string& sectionname, const std::string& keyname);

private:
    void parse(const std::string& filename);

    std::list<section> sections;
};

#endif //CONFIG_HPP_


//#include <string>

//class ConfigManager
//{
//public:
//	ConfigManager (std::string configData_ = nullptr) : m_configData(configData_) {};

//private:
//	std::string m_configData;

//};

//resolution=980x2090 
	//vsync=0

	//{resolution: 1920x1080, vsync=0}


	/*
	data
	{
		resolution:
		{
			width: 1920,
			height: 1080,
		},
		vsync: 0,
		box_attributes:
		{

		}
	}
	*/
	/*
	resolution = 1920x1080;
	vsync = 1;
	box_attributes = width : 30, length : 30, color : FFFFFFF;
	*/
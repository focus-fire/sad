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
    //ConfigManager(const std::string& filename);

    static ConfigManager& GetInstance();

    ConfigManager(const ConfigManager&) = delete;

    section* get_section(const std::string& sectionname);
    std::list<section>& get_sections();

    static section* getsection(const std::string& sectionname);

    std::string get_value(const std::string& sectionname, const std::string& keyname);

    static std::string getvalue(const std::string& sectionname, const std::string& keyname);

private:
    ConfigManager() {}

    static bool m_isFileRead;

    static void parse(const std::string& filename);

    static std::list<section> sections;
};

#endif //CONFIG_HPP_

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



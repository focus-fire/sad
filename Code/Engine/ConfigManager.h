#pragma once

#include <string>
#include <unordered_map>
#include <list>

namespace sad
{
    struct ConfigSection
    {
        std::string name;
        std::unordered_map<std::string, std::string> keyvalues;
    };

    class ConfigManager
    {
    public:
        ConfigManager(const ConfigManager&) = delete;

        static ConfigSection* GetSection(const std::string& sectionname);

        static std::string GetValue(const std::string& sectionname, const std::string& keyname);

        static ConfigManager& GetInstance();

    private:
        ConfigManager() {}

        static bool m_IsFileRead;

        static std::list<ConfigSection> sections;

        static void Parse(const std::string& filename);

        ConfigSection* MGetSection(const std::string& sectionname);

        std::list<ConfigSection>& MGetSections();

        std::string MGetValue(const std::string& sectionname, const std::string& keyname);
    };
}

#pragma once

#include <string>
#include <unordered_map>
#include <list>

namespace sad
{
    struct ConfigSection
    {
        std::string name;
        std::unordered_map<std::string, std::string> keyValues;
    };

    class ConfigManager
    {
    public:
        ConfigManager(const ConfigManager&) = delete;

        static ConfigManager& GetInstance();

        static ConfigSection* GetSection(const std::string& sectionName);
        static std::string GetValue(const std::string& sectionName, const std::string& keyName);

    private:
        ConfigManager() {}

        static bool m_IsFileRead;
        static std::list<ConfigSection> m_ConfigSections;

        static void Parse(const std::string& filename);

        ConfigSection* MGetSection(const std::string& sectionName);
        std::list<ConfigSection>& MGetSections();
        std::string MGetValue(const std::string& sectionName, const std::string& keyName);
    };
}

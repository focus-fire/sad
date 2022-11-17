#include "sadpch.h"

#include "ConfigManager.h"

std::list<sad::ConfigSection> sad::ConfigManager::m_ConfigSections;
bool sad::ConfigManager::m_IsFileRead = false;

sad::ConfigSection* sad::ConfigManager::MGetSection(const std::string& sectionName)
{
    std::list<ConfigSection>::iterator found = std::find_if(m_ConfigSections.begin(), m_ConfigSections.end(),
        [sectionName](const ConfigSection& sect) { 
            return core::StringUtils::Equals(sect.name, sectionName); 
        });

    return found != m_ConfigSections.end() ? &*found : NULL;
}

std::list<sad::ConfigSection>& sad::ConfigManager::MGetSections()
{
    return m_ConfigSections;
}

std::string sad::ConfigManager::MGetValue(const std::string& sectionName, const std::string& keyName)
{
    ConfigSection* sect = MGetSection(sectionName);

    if (sect != NULL) 
    {
        std::unordered_map<std::string, std::string>::const_iterator it = sect->keyValues.find(keyName);
        if (it != sect->keyValues.end())
            return it->second;
    }

    return "";
}

void sad::ConfigManager::Parse(const std::string& fileName)
{
    ConfigSection currentSection;
    std::ifstream fileStream;
    fileStream.open(fileName);

    if (!fileStream)
        throw std::invalid_argument(fileName + " could not be opened");

    for (std::string line; std::getline(fileStream, line);)
    {
        // if a comment
        if (!line.empty() && (line[0] == ';' || line[0] == '#')) 
        {
            // allow both ; and # comments at the start of a line
        }
        else if (line[0] == '[') 
        {
            /* A "[section]" line */
            size_t end = line.find_first_of(']');
            if (end != std::string::npos)
            {
                // this is a new section so if we have a current section populated, add it to list
                if (!currentSection.name.empty()) 
                {
                    m_ConfigSections.push_back(currentSection);  // copy
                    currentSection.name.clear();  // clear section for re-use
                    currentSection.keyValues.clear();
                }
                currentSection.name = line.substr(1, end - 1);
            }
            else 
            {
                // section has no closing ] char
            }
        }
        else if (!line.empty()) 
        {
            /* Not a comment, must be a name[=:]value pair */
            size_t end = line.find_first_of("=:");
            if (end != std::string::npos) 
            {
                std::string name = line.substr(0, end);
                std::string value = line.substr(end + 1);

                std::string sanitizedName = core::StringUtils::Trim(name);
                std::string sanitizedValue = core::StringUtils::Trim(value);
                currentSection.keyValues[sanitizedName] = sanitizedValue;
            }
            else 
            {
                // no key value delimitter
            }
        }
    } 

    // if we are out of loop we add last section
    // this is a new section so if we have a current section populated, add it to list
    if (!currentSection.name.empty()) 
    {
        m_ConfigSections.push_back(currentSection);  // copy
        currentSection.name = "";
        currentSection.keyValues.clear();
    }
}

sad::ConfigManager& sad::ConfigManager::GetInstance()
{
    if(!m_IsFileRead)
    {
        std::string configPath = core::FileUtils::GetPathInsideCodeDirectory("config.ini");

        Parse(configPath);
        m_IsFileRead = true;
    }

    static ConfigManager instance;
    return instance;
}

sad::ConfigSection* sad::ConfigManager::GetSection(const std::string& sectionname)
{
    return GetInstance().MGetSection(sectionname);
}

std::string sad::ConfigManager::GetValue(const std::string& sectionname, const std::string& keyname)
{
    return GetInstance().MGetValue(sectionname, keyname);
}
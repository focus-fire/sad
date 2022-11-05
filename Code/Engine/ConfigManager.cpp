#include "sadpch.h"

#include "ConfigManager.h"

std::list<sad::ConfigSection> sad::ConfigManager::sections;
bool sad::ConfigManager::m_IsFileRead = false;

sad::ConfigSection* sad::ConfigManager::MGetSection(const std::string& sectionname)
{
    std::list<ConfigSection>::iterator found = std::find_if(sections.begin(), sections.end(),
        [sectionname](const ConfigSection& sect) { 
            return sect.name.compare(sectionname) == 0; 
        });
    return found != sections.end() ? &*found : NULL;
}

std::list<sad::ConfigSection>& sad::ConfigManager::MGetSections()
{
    return sections;
}

std::string sad::ConfigManager::MGetValue(const std::string& sectionname, const std::string&keyname)
{
    ConfigSection* sect = MGetSection(sectionname);
    if (sect != NULL) 
    {
        std::unordered_map<std::string, std::string>::const_iterator it = sect->keyvalues.find(keyname);
        if (it != sect->keyvalues.end())
            return it->second;
    }
    return "";
}

void sad::ConfigManager::Parse(const std::string& filename)
{
    ConfigSection currentsection;
    std::ifstream fstrm;
    fstrm.open(filename);

    if (!fstrm)
        throw std::invalid_argument(filename + " could not be opened");

    for (std::string line; std::getline(fstrm, line);)
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
                if (!currentsection.name.empty()) 
                {
                    sections.push_back(currentsection);  // copy
                    currentsection.name.clear();  // clear section for re-use
                    currentsection.keyvalues.clear();
                }
                currentsection.name = line.substr(1, end - 1);
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
                core::StringUtils::LeftTrim(core::StringUtils::RightTrim(name));
                core::StringUtils::LeftTrim(core::StringUtils::RightTrim(value));

                currentsection.keyvalues[name] = value;
            }
            else 
            {
                // no key value delimitter
            }
        }
    } // for

    // if we are out of loop we add last section
    // this is a new section so if we have a current section populated, add it to list
    if (!currentsection.name.empty()) 
    {
        sections.push_back(currentsection);  // copy
        currentsection.name = "";
        currentsection.keyvalues.clear();
    }
}

sad::ConfigManager& sad::ConfigManager::GetInstance()
{
    if(!m_IsFileRead)
    {
        std::string configPath = core::FileUtils::GetCodeDirectory();
        configPath += core::FileUtils::ConvertOSPathString("/config.ini");
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
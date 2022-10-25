#include "sadpch.h"

#include "JsonManager.h"
#include <vector>

using json = nlohmann::json;
//Constructor
sad::JsonManager::JsonManager(){}

/**
 * @brief 
 * return the entire json data
 * @sample
 * std::string jsonPath = core::FileUtils::GetDataDirectory() + core::FileUtils::ConvertOSPathString("/TestFolder/sample.json");
 * @param path 
 * @return 
*/
json sad::JsonManager::GetJson(std::string path)
{
	std::string fileContent = core::FileUtils::ReadFile(path);
	return json::parse(fileContent);
}
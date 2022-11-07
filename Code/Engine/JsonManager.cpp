#include "sadpch.h"

#include "JsonManager.h"

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

/**
 * @brief 
 * Adds entities of the last saved level
 * Level 0 is the default location
 * saveFile default value is 1
*/
bool sad::JsonManager::ImportLevel(int saveFile)
{
	//if the savefile location is out of range
	if (0 > saveFile && saveFile < 100)
		return false;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << saveFile;
	std::string level_path = core::FileUtils::GetPathInsideDataDirectory("/Save/SaveFile" + ss.str() + ".json");

	if (!core::FileUtils::PathExists(level_path))
		return false;

	std::string json_output = core::FileUtils::ReadFile(level_path);

	NJSONInputArchive json_in(json_output);
	entt::basic_snapshot_loader loader(sad::ecs::Registry::GetEntityWorld());
	loader.entities(json_in).component<COMPONENT_TYPES>(json_in);

	return true;
}

/**
 * @brief 
 * Saves entities of the level into a json save file
 * saveFile default value is 1
*/
void sad::JsonManager::ExportLevel(int saveFile)
{
	//prevent default save file from changing
	if (saveFile == 0)
		return;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << saveFile;
	std::string level_path = core::FileUtils::GetPathInsideDataDirectory("/Save/SaveFile" + ss.str() + ".json");

	NJSONOutputArchive json_archive;
	entt::basic_snapshot snapshot(sad::ecs::Registry::GetEntityWorld());
	snapshot.entities(json_archive).component<COMPONENT_TYPES>(json_archive);
	json_archive.Close();

	std::string json_output = json_archive.AsString();
	core::Log(ELogType::Info, "Export ran");
	core::FileUtils::WriteFile(level_path, json_output);
}
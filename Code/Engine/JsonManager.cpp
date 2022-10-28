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

void sad::JsonManager::ImportLevel()
{
	std::string level_path = core::FileUtils::ReadFile(core::FileUtils::GetDataDirectory() + core::FileUtils::ConvertOSPathString("/Save/SaveFile01.json"));
	std::string json_output = core::FileUtils::ReadFile(level_path);
	NJSONInputArchive json_in(json_output);
	entt::basic_snapshot_loader loader(sad::ecs::Registry::GetEntityWorld());
	//loader.entities(json_in).component<Tower, Walker, Transform>(json_in);
}
void sad::JsonManager::ExportLevel()
{
	std::string level_path = core::FileUtils::ReadFile(core::FileUtils::GetDataDirectory() + core::FileUtils::ConvertOSPathString("/Save/SaveFile01.json"));
	NJSONOutputArchive json_archive;
	entt::basic_snapshot snapshot(sad::ecs::Registry::GetEntityWorld());
	//snapshot.entities(json_archive).component<Tower, Walker, Transform>(json_archive);
	json_archive.Close();

	std::string json_output = json_archive.AsString();
	if (!core::FileUtils::OverWriteExistingFile(level_path, json_output)) {
		core::FileUtils::CreateNewFile(level_path, json_output);
	}
}
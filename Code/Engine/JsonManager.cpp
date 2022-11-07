#include <sadpch.h>

#include "NJSONInputArchive.h"
#include "NJSONOutputArchive.h"
#include "Engine/ECS/Registry.h"
#include "Engine/ECS/Components/ComponentTypes.h"

#include <iomanip>
#include <nlohmann/json.hpp>

#include "JsonManager.h"

using json = nlohmann::json;

/**
 * @brief 
 * Adds entities of the last saved level
 * Level 0 is the default location
 * saveFile default value is 1
*/
bool sad::JsonManager::ImportLevel(int saveFile)
{
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << saveFile;
	std::string level_path = core::FileUtils::GetPathInsideDataDirectory("/Save/SaveFile" + ss.str() + ".json");

	if (!core::FileUtils::PathExists(level_path))
	{
		core::Log(ELogType::Error, "Save file doesn't exist");
		return false;
	}

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
bool sad::JsonManager::ExportLevel(int saveFile)
{
	//prevent default save file from changing
	if (saveFile == 0)
	{
		core::Log(ELogType::Error, "SaveFile00.json is the default save file, don't change this");
		return false;
	}
	else if (0 > saveFile && saveFile < 100)
	{
		core::Log(ELogType::Error, "The inputed save file id is out of range");
		return false;
	}

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

	return true;
}